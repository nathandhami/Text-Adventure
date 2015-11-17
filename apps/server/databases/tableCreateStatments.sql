CREATE TABLE users (
  userID integer primary key,
  userName varchar(30) unique,
  password varchar(20) not null,
  authencationLevel integer not null,
  signedOn integer default 0
);

CREATE TABLE characters (
  charID integer primary key,
  name varchar(30) unique,
  userID integer,
  location integer,
  FOREIGN KEY(userID) REFERENCES users(userID),
  FOREIGN KEY(location) REFERENCES zones(zoneID)
);

CREATE TABLE charactersOnline (
  charID integer primary key,
  sessionID text unique,
  inCombat integer,
  FOREIGN KEY(charID) REFERENCES characters(charID)
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
  inCombat integer,
  FOREIGN KEY(npcID) REFERENCES npcs(npcID),
  FOREIGN KEY(zoneID) REFERENCES zones(zoneID)
);

CREATE TABLE resetCommands (
  resetID integer primary key,
  action text,
  id integer,
  slot integer,
  npcLimit integer,
  room integer,
  state text,
  container integer
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

CREATE TABLE zones(
	zoneID integer primary key autoincrement,
	zoneName varchar(30),
	zoneDescription text
);

CREATE TABLE zone_ext_descriptions(
	descriptionID integer primary key autoincrement,
	zoneID integer,
	description text,
	keywords text,
	FOREIGN KEY(zoneID) REFERENCES zones(zoneID) on delete cascade
);

CREATE TABLE doors(
	doorID integer primary key,
	zoneID integer,
	description text,
	keywords text,
	direction varchar(30),
	linksTo integer,
	FOREIGN KEY(zoneID) REFERENCES zones_n(zoneID) on delete cascade,
	FOREIGN KEY(linksTo) REFERENCES zones_n(zoneID) on delete cascade
);

CREATE TABLE items(
	itemID integer primary key autoincrement,
	shortDescription text,
	description text,
	longDescription text,
	keywords text,
	isPickable integer,
	isEquippable integer,
	isStackable integer,
	isContainer integer
);

CREATE TABLE player_inventory(
	ownershipID integer primary key,
	charID integer,
	itemID integer,
	quantity integer,
	isEquipped integer,
	FOREIGN KEY(charID) REFERENCES characters(charID) on delete cascade,
	FOREIGN KEY(itemID) REFERENCES items(itemID) on delete cascade
);

CREATE TABLE npc_inventory(
  ownershipID integer primary key,
  npcInstanceID integer,
  itemID integer,
  quantity integer,
  isEquipped integer,
  FOREIGN KEY(npcInstanceID) REFERENCES instanceOfNpc(npcInstanceID) on delete cascade,
  FOREIGN KEY(itemID) REFERENCES items(itemID) on delete cascade
);


CREATE TABLE instanceOfItem (
	itemInstanceID integer primary key,
	itemID integer,
	zoneID integer,
	containerID integer,
	FOREIGN KEY(itemID) REFERENCES items(itemID),
	FOREIGN KEY(zoneID) REFERENCES zones(zoneID),
  FOREIGN KEY(containerID) REFERENCES instanceOfItem(itemInstanceID)
);
























