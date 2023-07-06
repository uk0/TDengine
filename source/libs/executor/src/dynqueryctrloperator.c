/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "executorInt.h"
#include "filter.h"
#include "function.h"
#include "operator.h"
#include "os.h"
#include "querynodes.h"
#include "querytask.h"
#include "tcompare.h"
#include "tdatablock.h"
#include "thash.h"
#include "tmsg.h"
#include "ttypes.h"
#include "dynqueryctrl.h"

int64_t gSessionId = 0;

static void destroyDynQueryCtrlOperator(void* param) {
  SDynQueryCtrlOperatorInfo* pDynCtrlOperator = (SDynQueryCtrlOperatorInfo*)param;
  taosMemoryFreeClear(param);
}

static FORCE_INLINE int32_t buildGroupCacheOperatorParam(SOperatorParam** ppRes, int32_t downstreamIdx, bool needCache, void* pGrpValue, int32_t grpValSize, SOperatorParam* pChild) {
  *ppRes = taosMemoryMalloc(sizeof(SOperatorParam));
  if (NULL == *ppRes) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  (*ppRes)->pOpParams = taosArrayInit(1, sizeof(SOperatorSpecParam));
  if (NULL == *ppRes) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }

  SGcOperatorParam* pGc = taosMemoryMalloc(sizeof(SGcOperatorParam));
  if (NULL == pGc) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }

  pGc->pChild = pChild;
  pGc->sessionId = atomic_add_fetch_64(&gSessionId, 1);
  pGc->downstreamIdx = downstreamIdx;
  pGc->needCache = needCache;
  pGc->pGroupValue = pGrpValue;
  pGc->groupValueSize = grpValSize;

  SOperatorSpecParam specParam;
  specParam.opType = QUERY_NODE_PHYSICAL_PLAN_GROUP_CACHE;
  specParam.value = pGc;

  taosArrayPush((*ppRes)->pOpParams, &specParam);

  return TSDB_CODE_SUCCESS;
}

static FORCE_INLINE int32_t buildExchangeOperatorParam(SOperatorParam** ppRes, int32_t* pVgId, int64_t* pUid, SOperatorParam* pChild) {
  *ppRes = taosMemoryMalloc(sizeof(SOperatorParam));
  if (NULL == *ppRes) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  (*ppRes)->pOpParams = taosArrayInit(1, sizeof(SOperatorSpecParam));
  if (NULL == *ppRes) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  
  SExchangeOperatorParam* pExc = taosMemoryMalloc(sizeof(SExchangeOperatorParam));
  if (NULL == pExc) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  
  pExc->pChild = pChild;
  pExc->vgId = *pVgId;
  pExc->uidList = taosArrayInit(1, sizeof(int64_t));
  if (NULL == pExc->uidList) {
    taosMemoryFree(pExc);
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  taosArrayPush(pExc->uidList, pUid);

  SOperatorSpecParam specParam;
  specParam.opType = QUERY_NODE_PHYSICAL_PLAN_EXCHANGE;
  specParam.value = pExc;

  taosArrayPush((*ppRes)->pOpParams, &specParam);

  return TSDB_CODE_SUCCESS;
}

static FORCE_INLINE int32_t buildMergeJoinOperatorParam(SOperatorParam** ppRes, SOperatorParam* pChild0, SOperatorParam* pChild1) {
  *ppRes = taosMemoryMalloc(sizeof(SOperatorParam));
  if (NULL == *ppRes) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  (*ppRes)->pOpParams = taosArrayInit(2, sizeof(SOperatorSpecParam));
  if (NULL == *ppRes) {
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  
  SSortMergeJoinOperatorParam* pJoin0 = taosMemoryMalloc(sizeof(SSortMergeJoinOperatorParam));
  SSortMergeJoinOperatorParam* pJoin1 = taosMemoryMalloc(sizeof(SSortMergeJoinOperatorParam));
  if (NULL == pJoin0 || NULL == pJoin1) {
    taosMemoryFree(pJoin0);
    taosMemoryFree(pJoin1);
    return TSDB_CODE_OUT_OF_MEMORY;
  }
  
  pJoin0->pChild = pChild0;
  pJoin1->pChild = pChild1;

  SOperatorSpecParam specParam;
  specParam.opType = QUERY_NODE_PHYSICAL_PLAN_MERGE_JOIN;
  specParam.value = pJoin0;
  taosArrayPush((*ppRes)->pOpParams, &specParam);
  specParam.value = pJoin1;
  taosArrayPush((*ppRes)->pOpParams, &specParam);

  return TSDB_CODE_SUCCESS;
}


static int32_t buildStbJoinOperatorParam(SDynQueryCtrlOperatorInfo* pInfo, SStbJoinPrevJoinCtx* pPrev, SOperatorParam** ppParam) {
  int32_t                     rowIdx = pPrev->lastRow + 1;
  SColumnInfoData*            pVg0 = taosArrayGet(pPrev->pLastBlk->pDataBlock, pInfo->stbJoin.basic.vgSlot[0]);
  SColumnInfoData*            pVg1 = taosArrayGet(pPrev->pLastBlk->pDataBlock, pInfo->stbJoin.basic.vgSlot[1]);
  SColumnInfoData*            pUid0 = taosArrayGet(pPrev->pLastBlk->pDataBlock, pInfo->stbJoin.basic.uidSlot[0]);
  SColumnInfoData*            pUid1 = taosArrayGet(pPrev->pLastBlk->pDataBlock, pInfo->stbJoin.basic.uidSlot[1]);
  SOperatorParam*             pExcParam0 = NULL;
  SOperatorParam*             pExcParam1 = NULL;
  SOperatorParam*             pGcParam0 = NULL;
  SOperatorParam*             pGcParam1 = NULL;
  
  int32_t code = buildExchangeOperatorParam(&pExcParam0, pVg0->pData + pVg0->info.bytes * rowIdx, pUid0->pData + pUid0->info.bytes * rowIdx, NULL);
  if (TSDB_CODE_SUCCESS == code) {
    code = buildExchangeOperatorParam(&pExcParam1, pVg1->pData + pVg1->info.bytes * rowIdx, pUid1->pData + pUid1->info.bytes * rowIdx, NULL);
  }
  if (TSDB_CODE_SUCCESS == code) {
    code = buildGroupCacheOperatorParam(&pGcParam0, 0, false, pUid0->pData + pUid0->info.bytes * rowIdx, pUid0->info.bytes, pExcParam0);
  }
  if (TSDB_CODE_SUCCESS == code) {
    code = buildGroupCacheOperatorParam(&pGcParam1, 1, false, pUid1->pData + pUid1->info.bytes * rowIdx, pUid1->info.bytes, pExcParam1);
  }
  if (TSDB_CODE_SUCCESS == code) {
    code = buildMergeJoinOperatorParam(ppParam, pGcParam0, pGcParam1);
  }
  return code;
}

static void seqJoinLaunchPostJoin(SOperatorInfo* pOperator, SSDataBlock** ppRes) {
  SDynQueryCtrlOperatorInfo* pInfo = pOperator->info;
  SStbJoinDynCtrlInfo*       pStbJoin = (SStbJoinDynCtrlInfo*)&pInfo->stbJoin;
  SStbJoinPrevJoinCtx*       pPrev = &pStbJoin->ctx.prev;
  SStbJoinPostJoinCtx*       pPost = &pStbJoin->ctx.post;
  SOperatorParam*            pParam = NULL;
  
  int32_t code = buildStbJoinOperatorParam(pInfo, pPrev, &pParam);
  if (TSDB_CODE_SUCCESS != code) {
    pOperator->pTaskInfo->code = code;
    T_LONG_JMP(pOperator->pTaskInfo->env, pOperator->pTaskInfo->code);
  }

  *ppRes = pOperator->pDownstream[1]->fpSet.getNextExtFn(pOperator->pDownstream[1], pParam);
  if (*ppRes) {
    pPost->isStarted = true;
  }
}

static void seqJoinWithSeqRetrieve(SOperatorInfo* pOperator, SSDataBlock** ppRes) {
  SDynQueryCtrlOperatorInfo* pInfo = pOperator->info;
  SStbJoinDynCtrlInfo*       pStbJoin = (SStbJoinDynCtrlInfo*)&pInfo->stbJoin;
  SStbJoinPrevJoinCtx*       pPrev = &pStbJoin->ctx.prev;

  while (true) {
    if ((pPrev->lastRow + 1) >= pPrev->pLastBlk->info.rows) {
      *ppRes = NULL;
      pPrev->pLastBlk = NULL;
      return;
    }

    seqJoinLaunchPostJoin(pOperator, ppRes);
    pPrev->lastRow++;
    if (*ppRes) {
      break;
    }
  }
}

static void seqJoinContinueRetrieve(SOperatorInfo* pOperator, SSDataBlock** ppRes) {
  SDynQueryCtrlOperatorInfo* pInfo = pOperator->info;
  SStbJoinDynCtrlInfo*       pStbJoin = (SStbJoinDynCtrlInfo*)&pInfo->stbJoin;
  SStbJoinPrevJoinCtx*       pPrev = &pStbJoin->ctx.prev;
  SStbJoinPostJoinCtx*       pPost = &pStbJoin->ctx.post;

  if (pPost->isStarted) {
    *ppRes = getNextBlockFromDownstream(pOperator, 1);
    if (NULL == *ppRes) {
      pPost->isStarted = false;
    } else {
      return;
    }
  }
  
  if (pStbJoin->ctx.prev.pLastBlk) {
    seqJoinWithSeqRetrieve(pOperator, ppRes);
  }
}

SSDataBlock* getResFromStbJoin(SOperatorInfo* pOperator) {
  SDynQueryCtrlOperatorInfo* pInfo = pOperator->info;
  SStbJoinDynCtrlInfo*       pStbJoin = (SStbJoinDynCtrlInfo*)&pInfo->stbJoin;
  SSDataBlock* pRes = NULL;

  seqJoinContinueRetrieve(pOperator, &pRes);
  if (pRes) {
    return pRes;
  }
  
  while (true) {
    SSDataBlock* pBlock = getNextBlockFromDownstream(pOperator, 0);
    if (NULL == pBlock) {
      break;
    }

    pStbJoin->ctx.prev.pLastBlk = pBlock;
    pStbJoin->ctx.prev.lastRow = -1;
    
    seqJoinWithSeqRetrieve(pOperator, &pRes);
    if (pRes) {
      break;
    }
  }

  return pRes;
}

SOperatorInfo* createDynQueryCtrlOperatorInfo(SOperatorInfo** pDownstream, int32_t numOfDownstream,
                                           SDynQueryCtrlPhysiNode* pPhyciNode, SExecTaskInfo* pTaskInfo) {
  SDynQueryCtrlOperatorInfo* pInfo = taosMemoryCalloc(1, sizeof(SDynQueryCtrlOperatorInfo));
  SOperatorInfo*     pOperator = taosMemoryCalloc(1, sizeof(SOperatorInfo));
  __optr_fn_t        nextFp = NULL;

  int32_t code = TSDB_CODE_SUCCESS;
  if (pOperator == NULL || pInfo == NULL) {
    code = TSDB_CODE_OUT_OF_MEMORY;
    goto _error;
  }

  code = appendDownstream(pOperator, pDownstream, numOfDownstream);
  if (TSDB_CODE_SUCCESS != code) {
    goto _error;
  }

  pInfo->qType = pPhyciNode->qType;
  switch (pInfo->qType) {
    case DYN_QTYPE_STB_HASH:
      memcpy(&pInfo->stbJoin, &pPhyciNode->stbJoin, sizeof(pPhyciNode->stbJoin));
      nextFp = getResFromStbJoin;
      break;
    default:
      qError("unsupported dynamic query ctrl type: %d", pInfo->qType);
      code = TSDB_CODE_INVALID_PARA;
      goto _error;
  }
  
  setOperatorInfo(pOperator, "DynQueryCtrlOperator", QUERY_NODE_PHYSICAL_PLAN_DYN_QUERY_CTRL, false, OP_NOT_OPENED, pInfo, pTaskInfo);

  pOperator->fpSet = createOperatorFpSet(optrDummyOpenFn, nextFp, NULL, destroyDynQueryCtrlOperator, optrDefaultBufFn, NULL, optrDefaultGetNextExtFn, NULL);

  return pOperator;

_error:
  if (pInfo != NULL) {
    destroyDynQueryCtrlOperator(pInfo);
  }

  taosMemoryFree(pOperator);
  pTaskInfo->code = code;
  return NULL;
}

