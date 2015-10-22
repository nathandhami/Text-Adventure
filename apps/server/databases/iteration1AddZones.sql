INSERT INTO zones VALUES (
  3001, 
  'The Temple Of Midgaard',
  '  You are in the southern end of the temple hall in the Temple of Midgaard. The temple has been constructed from giant marble blocks, eternal in appearance, and most of the walls are covered by ancient wall paintings picturing Gods, Giants and peasants. Large steps lead down through the grand temple gate, descending the huge mound upon which the temple is built and ends on the temple square below.',
  '- desc:
  - This entire world has been integrated, modified, debugged, and enhanced by
  - Hatchet, Kahn, and Furey of MERC Industries to be run by any MERC Mud.
  - 1992 December 17
  keywords:
  - plaque',
  '  - desc:\n- At the northern end of the temple hall is a statue and a huge altar.\ndir: north\nkeywords: []\nto: 3054\n- desc:\n- You see the temple square.\ndir: south\nkeywords: []\nto: 3005\n- desc:\n- You see the entrance to Mud School.\ndir: up\nkeywords: []\nto: 3700'
 );

INSERT INTO zones VALUES (
	3054,
	'By the Temple Altar',
	'   You are by the temple altar in the northern end of the Temple of Midgaard. A huge altar made from white polished marble is standing in front of you and behind it is a ten foot tall sitting statue of Odin, the King of the Gods.',
	'- desc:
    - Even though the altar is more than ten feet long it appears to be made from a
    - single block of white virgin marble.
    keywords:
    - altar
    - desc:
    - The statue represents the one-eyed Odin sitting on a his throne.  He has
    - long, grey hair and beard and a strict look on his face.  On top of the
    - throne, just above his shoulders, his two ravens Hugin and Munin are sitting
    - and at his feet are his wolves Gere and Freke.
    keywords:
    - statue
    - odin
    - king
    - god',
    '  - desc:
    - You see the southern end of the temple.
    dir: south
    keywords:
    temple
    to: 3001'
);

INSERT INTO zones VALUES (
	3005,
	'The Temple Square',
	'You are standing on the temple square.  Huge marble steps lead up to the temple gate.', 
	NULL,
  ' - desc:
    - You see the temple.
    dir: north
    keywords: []
    to: 3001
  - desc:
    - You see the good old Grunting Boar Inn.
    dir: east
    keywords: []
    to: 3006
  - desc:
    - You see the Market Square.
    dir: south
    keywords: []
    to: 3014
  - desc:
    - You see the entrance to the Clerics Guild.
    dir: west
    keywords: []
    to: 3004
  - desc:
    - You see the air.
    dir: up
    keywords: []
    to: 3057'
);