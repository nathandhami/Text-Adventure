CREATE TABLE users (
  userID integer primary key,
  userName varchar(30) unique,
  password varchar(20)
);

CREATE TABLE characters (
  charID integer primary key,
  name varchar(30) NOT NULL,
  userID int unique,
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
  southID integer,
  southDesc text,
  eastID integer,
  eastDesc text,
  westID integer,
  westDesc text,
  upID integer,
  upDesc text,
  downID integer,
  downDesc text
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