BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "cocktail" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"beschreibung"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "zutaten" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"deko"	BOOLEAN,
	"alkoholhaltig"	BOOLEAN,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "cocktail_list" (
	"id"	INTEGER NOT NULL,
	"cocktail_id"	INTEGER NOT NULL,
	"zutat_id"	INTEGER NOT NULL,
	"menge"	INTEGER NOT NULL,
	"einheit"	TEXT NOT NULL,
	FOREIGN KEY("cocktail_id") REFERENCES "cocktail"("id"),
	FOREIGN KEY("zutat_id") REFERENCES "zutaten"("id"),
	PRIMARY KEY("id" AUTOINCREMENT)
);
INSERT INTO "cocktail" VALUES (4,'Name','Beschreibung');
INSERT INTO "cocktail" VALUES (5,'esfrswef','sdfgsdf');
INSERT INTO "cocktail" VALUES (6,'dsfdsf','sdfsdf');
INSERT INTO "zutaten" VALUES (1,'Zucker Sirup',0,0);
INSERT INTO "zutaten" VALUES (2,'Zitronensaft',0,0);
INSERT INTO "zutaten" VALUES (3,'Limettensaft',0,0);
INSERT INTO "zutaten" VALUES (4,'Gin',0,1);
INSERT INTO "zutaten" VALUES (5,'Wodka',0,1);
INSERT INTO "zutaten" VALUES (6,'Aromatische Bitter',0,1);
INSERT INTO "zutaten" VALUES (7,'Cognac',0,1);
INSERT INTO "zutaten" VALUES (8,'Trockener Wermut',0,1);
INSERT INTO "zutaten" VALUES (9,'Rosso Wermut',0,1);
INSERT INTO "zutaten" VALUES (10,'Orangenbitter',0,1);
INSERT INTO "zutaten" VALUES (11,'Orangensaft',0,0);
INSERT INTO "zutaten" VALUES (12,'Triple Sec Likör',0,1);
INSERT INTO "zutaten" VALUES (13,'Ananassaft',0,0);
INSERT INTO "zutaten" VALUES (14,'Weißer Rum',0,1);
INSERT INTO "zutaten" VALUES (15,'Bourbon Whiskey',0,1);
INSERT INTO "zutaten" VALUES (16,'Eiweiß',0,0);
INSERT INTO "zutaten" VALUES (17,'Sodawasser',0,0);
INSERT INTO "zutaten" VALUES (18,'Tequila Reposado',0,1);
INSERT INTO "zutaten" VALUES (19,'Grenadine Sirup',0,0);
INSERT INTO "zutaten" VALUES (20,'Grapefruitsaft',0,0);
INSERT INTO "zutaten" VALUES (21,'Apfelsaft',0,0);
INSERT INTO "zutaten" VALUES (22,'Maraschino Likör',0,1);
INSERT INTO "zutaten" VALUES (23,'Absinth',0,1);
INSERT INTO "zutaten" VALUES (24,'Cranberrysaft',0,0);
INSERT INTO "zutaten" VALUES (25,'Italienischer roter Bitterlikör',0,1);
INSERT INTO "zutaten" VALUES (26,'Scotch Whisky',0,1);
INSERT INTO "zutaten" VALUES (27,'Sahne',0,0);
INSERT INTO "zutaten" VALUES (28,'Holunderblütenlikör',0,1);
INSERT INTO "zutaten" VALUES (29,'Brut Sekt',0,1);
INSERT INTO "zutaten" VALUES (30,'Cognac Orangenlikör',0,1);
INSERT INTO "zutaten" VALUES (31,'Calvados',0,1);
INSERT INTO "zutaten" VALUES (32,'Roggenwhiskey',0,1);
INSERT INTO "zutaten" VALUES (33,'Aprikosenlikör',0,1);
INSERT INTO "zutaten" VALUES (34,'Minzblätter',1,0);
INSERT INTO "zutaten" VALUES (35,'Creole Bitter',0,1);
INSERT INTO "zutaten" VALUES (36,'Goldener Rum',0,1);
INSERT INTO "zutaten" VALUES (37,'Kaffeelikör',0,1);
INSERT INTO "zutaten" VALUES (38,'Bénédictine D.O.M. Likör',0,1);
INSERT INTO "zutaten" VALUES (39,'Amaretto Likör',0,1);
INSERT INTO "zutaten" VALUES (40,'Gut gereifter Rum',0,1);
INSERT INTO "zutaten" VALUES (41,'Orangen Curaçao Likör',0,1);
INSERT INTO "zutaten" VALUES (42,'Chartreuse Verte',0,1);
INSERT INTO "zutaten" VALUES (43,'Mezcal',0,1);
INSERT INTO "zutaten" VALUES (44,'Crème de Cacao Likör',0,1);
INSERT INTO "zutaten" VALUES (45,'Kirschlikör',0,1);
INSERT INTO "zutaten" VALUES (46,'Enzianlikör',0,1);
INSERT INTO "zutaten" VALUES (47,'Honig Sirup',0,0);
INSERT INTO "zutaten" VALUES (48,'Schwarzer Himbeerlikör',0,1);
INSERT INTO "zutaten" VALUES (49,'Orgeat Sirup',0,0);
INSERT INTO "zutaten" VALUES (50,'Blanco Tequila',0,1);
INSERT INTO "zutaten" VALUES (51,'Agavensirup',0,0);
INSERT INTO "zutaten" VALUES (52,'Salzlösung',0,0);
INSERT INTO "zutaten" VALUES (53,'Cachaça',0,1);
INSERT INTO "zutaten" VALUES (54,'Fino Sherry',0,1);
INSERT INTO "zutaten" VALUES (55,'Chartreuse Jaune',0,1);
INSERT INTO "zutaten" VALUES (56,'Ingwer Ale',0,0);
INSERT INTO "zutaten" VALUES (57,'Falernum Likör',0,1);
INSERT INTO "zutaten" VALUES (58,'Crème de Banane Likör',0,1);
INSERT INTO "zutaten" VALUES (59,'Lillet Blanc',0,1);
INSERT INTO "zutaten" VALUES (60,'Torfiger Scotch Whisky',0,1);
COMMIT;
