CREATE TABLE users (
  userID integer primary key,
  userName varchar(30) unique,
  password varchar(20) not null,
  authencationLevel integer not null
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
  otherItemInstanceID integer,
  FOREIGN KEY(itemID) REFERENCES items(itemID),
  FOREIGN KEY(charID) REFERENCES characters(charID),
  FOREIGN KEY(zoneID) REFERENCES zones(zoneID),
  FOREIGN KEY(npcInstanceID) REFERENCES instanceOfNpc(npcInstanceID),
  FOREIGN KEY(otherItemInstanceID) REFERENCES instanceOfItem(instanceID)
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
  isAlive integer,
  FOREIGN KEY(npcID) REFERENCES npcs(npcID),
  FOREIGN KEY(zoneID) REFERENCES zones(zoneID)
);

CREATE TABLE resetCommands (
  resetID integer not null,
  action text,
  id integer,
  slot integer,
  npcLimit integer,
  room integer,
  primary key(action, id, room),
  FOREIGN KEY(room) REFERENCES zones(zoneID)
);

CREATE TABLE playerAttributes (
  charID integer primary key,
  description text,
  level integer,
  experience integer,
  health integer,
  strength integer, 
  intelligence integer,
  dexterity integer,
  charisma integer,
  ringSlot integer,
  headSlot integer,
  chestSlot integer,
  greavesSlot integer,
  feetSlot integer,
  handSlot integer,
  weponSlot integer,
  FOREIGN KEY(charID) REFERENCES characters(charID) on delete cascade
);

CREATE TABLE npcAttributes (
  npcInstanceID integer primary key,
  level integer,
  experience integer,
  health integer,
  strength integer, 
  intelligence integer,
  dexterity integer,
  charisma integer,
  ringSlot integer,
  headSlot integer,
  chestSlot integer,
  greavesSlot integer,
  feetSlot integer,
  handSlot integer,
  weponSlot integer,
  FOREIGN KEY(npcInstanceID) REFERENCES instanceOfNpc(npcInstanceID) on delete cascade
);