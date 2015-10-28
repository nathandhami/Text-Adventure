CREATE TABLE users (
  userID integer primary key,
  userName varchar(30) unique,
  password varchar(20)
);

CREATE TABLE characters (
  charID integer primary key,
  name varchar(30) NOT NULL,
  userID integer,
  location integer,
  FOREIGN KEY(userID) REFERENCES users(userID),
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
  longDesc text,
  shortDesc text
);

CREATE TABLE instanceOfItem (
  instanceID integer primary key,
  itemID integer,
  charID integer,
  zoneID integer,
  npcInstanceID integer,
  FOREIGN KEY(itemID) REFERENCES items(itemID),
  FOREIGN KEY(charID) REFERENCES characters(charID),
  FOREIGN KEY(zoneID) REFERENCES zones(zoneID),
  FOREIGN KEY(npcInstanceID) REFERENCES instanceOfNpc(npcInstanceID)
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

CREATE TABLE instanceOfNpc (
  npcInstanceID integer primary key,
  npcID integer,
  zoneID integer,
  FOREIGN KEY(npcID) REFERENCES npcs(npcID),
  FOREIGN KEY(zoneID) REFERENCES zones(zoneID)
);