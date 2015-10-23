CREATE TABLE users (
  userID integer primary key,
  userName varchar(30) unique,
  password varchar(20)
);

CREATE TABLE characters (
  charID integer primary key,
  name varchar(30) NOT NULL,
  userID int unique,
  location integer,
  FOREIGN KEY(userID) REFERENCES user(userID),
  FOREIGN KEY(location) REFERENCES zones(zoneID)
);

CREATE TABLE charactersOnline (
  charID integer primary key,
  sessionID text unique,
  FOREIGN KEY(charID) REFERENCES characters(charID)
);

CREATE TABLE items (
  itemID integer primary key,
  extendedDesc text,
  keywords text,
  description text
);

CREATE TABLE instanceOfItem (
  instanceID integer primary key,
  itemID integer,
  charID integer,
  zoneID, integer,
  FOREIGN KEY(itemID) REFERENCES items(itemID),
  FOREIGN KEY(charID) REFERENCES characters(charID),
  FOREIGN KEY(zoneID) REFERENCES zones(zoneID)
);

CREATE TABLE zones(
  zoneID integer primary key,
  zoneName varchar(30),
  description text,
  extendedDesc text,
  doors text
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