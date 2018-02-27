
#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#define	t_ACCESS	258
#define	t_AFTER	259
#define	t_ALIAS	260
#define	t_ALL	261
#define	t_AND	262
#define	t_ARCHITECTURE	263
#define	t_ARRAY	264
#define	t_ASSERT	265
#define	t_ATTRIBUTE	266
#define	t_BEGIN	267
#define	t_BLOCK	268
#define	t_BODY	269
#define	t_BUFFER	270
#define	t_BUS	271
#define	t_CASE	272
#define	t_COMPONENT	273
#define	t_CONFIGURATION	274
#define	t_CONSTANT	275
#define	t_DISCONNECT	276
#define	t_DOWNTO	277
#define	t_ELSE	278
#define	t_ELSIF	279
#define	t_END	280
#define	t_ENTITY	281
#define	t_EXIT	282
#define	t_FILE	283
#define	t_FOR	284
#define	t_FUNCTION	285
#define	t_GENERATE	286
#define	t_GENERIC	287
#define	t_GUARDED	288
#define	t_IF	289
#define	t_IN	290
#define	t_INOUT	291
#define	t_IS	292
#define	t_LABEL	293
#define	t_LIBRARY	294
#define	t_LINKAGE	295
#define	t_LOOP	296
#define	t_MAP	297
#define	t_NAND	298
#define	t_NEW	299
#define	t_NEXT	300
#define	t_NOR	301
#define	t_NULL	302
#define	t_OF	303
#define	t_ON	304
#define	t_OPEN	305
#define	t_OR	306
#define	t_OTHERS	307
#define	t_OUT	308
#define	t_PACKAGE	309
#define	t_PORT	310
#define	t_PROCEDURE	311
#define	t_PROCESS	312
#define	t_RANGE	313
#define	t_RECORD	314
#define	t_REGISTER	315
#define	t_REPORT	316
#define	t_RETURN	317
#define	t_SELECT	318
#define	t_SEVERITY	319
#define	t_SIGNAL	320
#define	t_SUBTYPE	321
#define	t_THEN	322
#define	t_TO	323
#define	t_TRANSPORT	324
#define	t_TYPE	325
#define	t_UNITS	326
#define	t_UNTIL	327
#define	t_USE	328
#define	t_VARIABLE	329
#define	t_WAIT	330
#define	t_WHEN	331
#define	t_WHILE	332
#define	t_WITH	333
#define	t_XOR	334
#define	t_EQSym	335
#define	t_NESym	336
#define	t_LTSym	337
#define	t_LESym	338
#define	t_GTSym	339
#define	t_GESym	340
#define	t_Plus	341
#define	t_Minus	342
#define	t_Ampersand	343
#define	MED_PRECEDENCE	344
#define	t_Star	345
#define	t_Slash	346
#define	t_MOD	347
#define	t_REM	348
#define	t_DoubleStar	349
#define	t_ABS	350
#define	t_NOT	351
#define	MAX_PRECEDENCE	352
#define	t_Apostrophe	353
#define	t_LeftParen	354
#define	t_RightParen	355
#define	t_Comma	356
#define	t_VarAsgn	357
#define	t_Colon	358
#define	t_Semicolon	359
#define	t_Arrow	360
#define	t_Box	361
#define	t_Bar	362
#define	t_Dot	363
#define	t_AbstractLit	364
#define	t_CharacterLit	365
#define	t_StringLit	366
#define	t_Identifier	367

