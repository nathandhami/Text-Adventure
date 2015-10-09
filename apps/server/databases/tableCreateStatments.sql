CREATE TABLE users (
  userID varchar(30) primary key,
  password varchar(20)
);

CREATE TABLE characters (
  charID integer primary key,
  name varchar(30) NOT NULL,
  userID varchar(30),
  lastLocation integer,
  isOnline integer,
  FOREIGN KEY(userID) REFERENCES user(userID),
  FOREIGN KEY(lastLocation) REFERENCES zones(zoneID)
);

CREATE TABLE zones(
  zoneID integer primary key,
  zoneName varchar(30),
  description text,
  extendedDesc text,
  northID integer,
  northDesc text,
  northKWs text,
  southID integer,
  southDesc text,
  southKWs text,
  eastID integer,
  eastDesc text,
  eastKWs text,
  westID integer,
  westDesc text,
  westKWs text
);

CREATE TABLE npcs (
  npcID integer primary key,
  description text,
  keywords text,
  longDesc text,
  shortDesc varchar(30)
);

CREATE TABLE populated_by (
  npcID integer,
  zoneID integer,
  FOREIGN KEY(npcID) REFERENCES npcs(npcID),
  FOREIGN KEY(zoneID) REFERENCES zones(zoneID)
);