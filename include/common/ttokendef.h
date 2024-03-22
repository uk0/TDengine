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

#ifndef _TD_COMMON_TOKEN_H_
#define _TD_COMMON_TOKEN_H_


#define TK_OR                               1
#define TK_AND                              2
#define TK_UNION                            3
#define TK_ALL                              4
#define TK_MINUS                            5
#define TK_EXCEPT                           6
#define TK_INTERSECT                        7
#define TK_NK_BITAND                        8
#define TK_NK_BITOR                         9
#define TK_NK_LSHIFT                       10
#define TK_NK_RSHIFT                       11
#define TK_NK_PLUS                         12
#define TK_NK_MINUS                        13
#define TK_NK_STAR                         14
#define TK_NK_SLASH                        15
#define TK_NK_REM                          16
#define TK_NK_CONCAT                       17
#define TK_CREATE                          18
#define TK_ACCOUNT                         19
#define TK_NK_ID                           20
#define TK_PASS                            21
#define TK_NK_STRING                       22
#define TK_ALTER                           23
#define TK_PPS                             24
#define TK_TSERIES                         25
#define TK_STORAGE                         26
#define TK_STREAMS                         27
#define TK_QTIME                           28
#define TK_DBS                             29
#define TK_USERS                           30
#define TK_CONNS                           31
#define TK_STATE                           32
#define TK_NK_COMMA                        33
#define TK_HOST                            34
#define TK_USER                            35
#define TK_ENABLE                          36
#define TK_NK_INTEGER                      37
#define TK_SYSINFO                         38
#define TK_ADD                             39
#define TK_DROP                            40
#define TK_GRANT                           41
#define TK_ON                              42
#define TK_TO                              43
#define TK_REVOKE                          44
#define TK_FROM                            45
#define TK_SUBSCRIBE                       46
#define TK_READ                            47
#define TK_WRITE                           48
#define TK_NK_DOT                          49
#define TK_WITH                            50
#define TK_DNODE                           51
#define TK_PORT                            52
#define TK_DNODES                          53
#define TK_RESTORE                         54
#define TK_NK_IPTOKEN                      55
#define TK_FORCE                           56
#define TK_UNSAFE                          57
#define TK_CLUSTER                         58
#define TK_LOCAL                           59
#define TK_QNODE                           60
#define TK_BNODE                           61
#define TK_SNODE                           62
#define TK_MNODE                           63
#define TK_VNODE                           64
#define TK_DATABASE                        65
#define TK_USE                             66
#define TK_FLUSH                           67
#define TK_TRIM                            68
#define TK_COMPACT                         69
#define TK_IF                              70
#define TK_NOT                             71
#define TK_EXISTS                          72
#define TK_BUFFER                          73
#define TK_CACHEMODEL                      74
#define TK_CACHESIZE                       75
#define TK_COMP                            76
#define TK_DURATION                        77
#define TK_NK_VARIABLE                     78
#define TK_MAXROWS                         79
#define TK_MINROWS                         80
#define TK_KEEP                            81
#define TK_PAGES                           82
#define TK_PAGESIZE                        83
#define TK_TSDB_PAGESIZE                   84
#define TK_PRECISION                       85
#define TK_REPLICA                         86
#define TK_VGROUPS                         87
#define TK_SINGLE_STABLE                   88
#define TK_RETENTIONS                      89
#define TK_SCHEMALESS                      90
#define TK_WAL_LEVEL                       91
#define TK_WAL_FSYNC_PERIOD                92
#define TK_WAL_RETENTION_PERIOD            93
#define TK_WAL_RETENTION_SIZE              94
#define TK_WAL_ROLL_PERIOD                 95
#define TK_WAL_SEGMENT_SIZE                96
#define TK_STT_TRIGGER                     97
#define TK_TABLE_PREFIX                    98
#define TK_TABLE_SUFFIX                    99
#define TK_KEEP_TIME_OFFSET               100
#define TK_NK_COLON                       101
#define TK_BWLIMIT                        102
#define TK_START                          103
#define TK_TIMESTAMP                      104
#define TK_END                            105
#define TK_TABLE                          106
#define TK_NK_LP                          107
#define TK_NK_RP                          108
#define TK_STABLE                         109
#define TK_COLUMN                         110
#define TK_MODIFY                         111
#define TK_RENAME                         112
#define TK_TAG                            113
#define TK_SET                            114
#define TK_NK_EQ                          115
#define TK_USING                          116
#define TK_TAGS                           117
#define TK_PRIMARY                        118
#define TK_KEY                            119
#define TK_BOOL                           120
#define TK_TINYINT                        121
#define TK_SMALLINT                       122
#define TK_INT                            123
#define TK_INTEGER                        124
#define TK_BIGINT                         125
#define TK_FLOAT                          126
#define TK_DOUBLE                         127
#define TK_BINARY                         128
#define TK_NCHAR                          129
#define TK_UNSIGNED                       130
#define TK_JSON                           131
#define TK_VARCHAR                        132
#define TK_MEDIUMBLOB                     133
#define TK_BLOB                           134
#define TK_VARBINARY                      135
#define TK_GEOMETRY                       136
#define TK_DECIMAL                        137
#define TK_COMMENT                        138
#define TK_MAX_DELAY                      139
#define TK_WATERMARK                      140
#define TK_ROLLUP                         141
#define TK_TTL                            142
#define TK_SMA                            143
#define TK_DELETE_MARK                    144
#define TK_FIRST                          145
#define TK_LAST                           146
#define TK_SHOW                           147
#define TK_PRIVILEGES                     148
#define TK_DATABASES                      149
#define TK_TABLES                         150
#define TK_STABLES                        151
#define TK_MNODES                         152
#define TK_QNODES                         153
#define TK_ARBGROUPS                      154
#define TK_FUNCTIONS                      155
#define TK_INDEXES                        156
#define TK_ACCOUNTS                       157
#define TK_APPS                           158
#define TK_CONNECTIONS                    159
#define TK_LICENCES                       160
#define TK_GRANTS                         161
#define TK_FULL                           162
#define TK_LOGS                           163
#define TK_MACHINES                       164
#define TK_QUERIES                        165
#define TK_SCORES                         166
#define TK_TOPICS                         167
#define TK_VARIABLES                      168
#define TK_BNODES                         169
#define TK_SNODES                         170
#define TK_TRANSACTIONS                   171
#define TK_DISTRIBUTED                    172
#define TK_CONSUMERS                      173
#define TK_SUBSCRIPTIONS                  174
#define TK_VNODES                         175
#define TK_ALIVE                          176
#define TK_VIEWS                          177
#define TK_VIEW                           178
#define TK_COMPACTS                       179
#define TK_NORMAL                         180
#define TK_CHILD                          181
#define TK_LIKE                           182
#define TK_TBNAME                         183
#define TK_QTAGS                          184
#define TK_AS                             185
#define TK_SYSTEM                         186
#define TK_INDEX                          187
#define TK_FUNCTION                       188
#define TK_INTERVAL                       189
#define TK_COUNT                          190
#define TK_LAST_ROW                       191
#define TK_META                           192
#define TK_ONLY                           193
#define TK_TOPIC                          194
#define TK_CONSUMER                       195
#define TK_GROUP                          196
#define TK_DESC                           197
#define TK_DESCRIBE                       198
#define TK_RESET                          199
#define TK_QUERY                          200
#define TK_CACHE                          201
#define TK_EXPLAIN                        202
#define TK_ANALYZE                        203
#define TK_VERBOSE                        204
#define TK_NK_BOOL                        205
#define TK_RATIO                          206
#define TK_NK_FLOAT                       207
#define TK_OUTPUTTYPE                     208
#define TK_AGGREGATE                      209
#define TK_BUFSIZE                        210
#define TK_LANGUAGE                       211
#define TK_REPLACE                        212
#define TK_STREAM                         213
#define TK_INTO                           214
#define TK_PAUSE                          215
#define TK_RESUME                         216
#define TK_TRIGGER                        217
#define TK_AT_ONCE                        218
#define TK_WINDOW_CLOSE                   219
#define TK_IGNORE                         220
#define TK_EXPIRED                        221
#define TK_FILL_HISTORY                   222
#define TK_UPDATE                         223
#define TK_SUBTABLE                       224
#define TK_UNTREATED                      225
#define TK_KILL                           226
#define TK_CONNECTION                     227
#define TK_TRANSACTION                    228
#define TK_BALANCE                        229
#define TK_VGROUP                         230
#define TK_LEADER                         231
#define TK_MERGE                          232
#define TK_REDISTRIBUTE                   233
#define TK_SPLIT                          234
#define TK_DELETE                         235
#define TK_INSERT                         236
#define TK_NK_BIN                         237
#define TK_NK_HEX                         238
#define TK_NULL                           239
#define TK_NK_QUESTION                    240
#define TK_NK_ALIAS                       241
#define TK_NK_ARROW                       242
#define TK_ROWTS                          243
#define TK_QSTART                         244
#define TK_QEND                           245
#define TK_QDURATION                      246
#define TK_WSTART                         247
#define TK_WEND                           248
#define TK_WDURATION                      249
#define TK_IROWTS                         250
#define TK_ISFILLED                       251
#define TK_CAST                           252
#define TK_NOW                            253
#define TK_TODAY                          254
#define TK_TIMEZONE                       255
#define TK_CLIENT_VERSION                 256
#define TK_SERVER_VERSION                 257
#define TK_SERVER_STATUS                  258
#define TK_CURRENT_USER                   259
#define TK_CASE                           260
#define TK_WHEN                           261
#define TK_THEN                           262
#define TK_ELSE                           263
#define TK_BETWEEN                        264
#define TK_IS                             265
#define TK_NK_LT                          266
#define TK_NK_GT                          267
#define TK_NK_LE                          268
#define TK_NK_GE                          269
#define TK_NK_NE                          270
#define TK_MATCH                          271
#define TK_NMATCH                         272
#define TK_CONTAINS                       273
#define TK_IN                             274
#define TK_JOIN                           275
#define TK_INNER                          276
#define TK_SELECT                         277
#define TK_NK_HINT                        278
#define TK_DISTINCT                       279
#define TK_WHERE                          280
#define TK_PARTITION                      281
#define TK_BY                             282
#define TK_SESSION                        283
#define TK_STATE_WINDOW                   284
#define TK_EVENT_WINDOW                   285
#define TK_COUNT_WINDOW                   286
#define TK_SLIDING                        287
#define TK_FILL                           288
#define TK_VALUE                          289
#define TK_VALUE_F                        290
#define TK_NONE                           291
#define TK_PREV                           292
#define TK_NULL_F                         293
#define TK_LINEAR                         294
#define TK_NEXT                           295
#define TK_HAVING                         296
#define TK_RANGE                          297
#define TK_EVERY                          298
#define TK_ORDER                          299
#define TK_SLIMIT                         300
#define TK_SOFFSET                        301
#define TK_LIMIT                          302
#define TK_OFFSET                         303
#define TK_ASC                            304
#define TK_NULLS                          305
#define TK_ABORT                          306
#define TK_AFTER                          307
#define TK_ATTACH                         308
#define TK_BEFORE                         309
#define TK_BEGIN                          310
#define TK_BITAND                         311
#define TK_BITNOT                         312
#define TK_BITOR                          313
#define TK_BLOCKS                         314
#define TK_CHANGE                         315
#define TK_COMMA                          316
#define TK_CONCAT                         317
#define TK_CONFLICT                       318
#define TK_COPY                           319
#define TK_DEFERRED                       320
#define TK_DELIMITERS                     321
#define TK_DETACH                         322
#define TK_DIVIDE                         323
#define TK_DOT                            324
#define TK_EACH                           325
#define TK_FAIL                           326
#define TK_FILE                           327
#define TK_FOR                            328
#define TK_GLOB                           329
#define TK_ID                             330
#define TK_IMMEDIATE                      331
#define TK_IMPORT                         332
#define TK_INITIALLY                      333
#define TK_INSTEAD                        334
#define TK_ISNULL                         335
#define TK_MODULES                        336
#define TK_NK_BITNOT                      337
#define TK_NK_SEMI                        338
#define TK_NOTNULL                        339
#define TK_OF                             340
#define TK_PLUS                           341
#define TK_PRIVILEGE                      342
#define TK_RAISE                          343
#define TK_RESTRICT                       344
#define TK_ROW                            345
#define TK_SEMI                           346
#define TK_STAR                           347
#define TK_STATEMENT                      348
#define TK_STRICT                         349
#define TK_STRING                         350
#define TK_TIMES                          351
#define TK_VALUES                         352
#define TK_VARIABLE                       353
#define TK_WAL                            354


#define TK_NK_SPACE         600
#define TK_NK_COMMENT       601
#define TK_NK_ILLEGAL       602
// #define TK_NK_HEX           603  // hex number  0x123
#define TK_NK_OCT           604  // oct number
// #define TK_NK_BIN           605  // bin format data 0b111
#define TK_BATCH_SCAN       606
#define TK_NO_BATCH_SCAN    607
#define TK_SORT_FOR_GROUP   608
#define TK_PARTITION_FIRST  609
#define TK_PARA_TABLES_SORT  610
#define TK_SMALLDATA_TS_SORT 611

#define TK_NK_NIL 65535

#endif /*_TD_COMMON_TOKEN_H_*/
