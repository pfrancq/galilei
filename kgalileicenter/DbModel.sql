
-- author Vandaele Valery     vavdaele@ulb.ac.be
--------------------------------------------------------------------------

-- this file contains all the command to be use to create a database 
-- with all required tables and fields.
--------------------------------------------------------------------------

-- This file can be modified but take care : the database created with this  
-- file must respect GALILEI exigences !!!

---------------------------------------------------------------------------- 
---------------------------------------------------------------------------- 
-- MODIFICATION OF THIS FILE CAN DRIVE TO THE INCAPACITY OF CREATE NEW EMPTY
-- DATABASES IN GALILEI ENVIRONEMENT !!!
---------------------------------------------------------------------------- 
---------------------------------------------------------------------------- 
  
--
-- Table structure for table 'engroupsbykwds'
--

CREATE TABLE engroupsbykwds (
  groupid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs double default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'enhtmlsbykwds'
--

CREATE TABLE enhtmlsbykwds (
  htmlid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs double(20,10) default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'enkwds'
--

CREATE TABLE enkwds (
  kwdid int(11) NOT NULL auto_increment,
  kwd varchar(50) default NULL,
  type int(11) default '1',
  PRIMARY KEY  (kwdid)
) TYPE=MyISAM;


--
-- Table structure for table 'enkwdsbygroups'
--

CREATE TABLE enkwdsbygroups (
  grid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0'
) TYPE=MyISAM;


--
-- Table structure for table 'enstopkwds'
--

CREATE TABLE enstopkwds (
  kwdid int(11) NOT NULL auto_increment,
  kwd varchar(50) NOT NULL default '',
  type int(11) default '1',
  PRIMARY KEY  (kwdid)
) TYPE=MyISAM;


--
-- Table structure for table 'ensubprofilesbykwds'
--

CREATE TABLE ensubprofilesbykwds (
  subprofileid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  weight float NOT NULL default '0'
) TYPE=MyISAM;


--
-- Table structure for table 'frgroupsbykwds'
--

CREATE TABLE frgroupsbykwds (
  groupid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs double default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'frhtmlsbykwds'
--

CREATE TABLE frhtmlsbykwds (
  htmlid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs int(20) default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'frkwds'
--

CREATE TABLE frkwds (
  kwdid int(11) NOT NULL auto_increment,
  kwd varchar(50) default NULL,
  type int(11) default '1',
  PRIMARY KEY  (kwdid)
) TYPE=MyISAM;


--
-- Table structure for table 'frkwdsbygroups'
--

CREATE TABLE frkwdsbygroups (
  grid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0'
) TYPE=MyISAM;


--
-- Table structure for table 'frstopkwds'
--

CREATE TABLE frstopkwds (
  kwdid int(11) NOT NULL auto_increment,
  kwd varchar(50) NOT NULL default '',
  type int(11) default '1',
  PRIMARY KEY  (kwdid)
) TYPE=MyISAM;


--
-- Table structure for table 'frsubprofilesbykwds'
--

CREATE TABLE frsubprofilesbykwds (
  subprofileid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  weight float NOT NULL default '0'
) TYPE=MyISAM;


--
-- Table structure for table 'nlgroupsbykwds'
--

CREATE TABLE nlgroupsbykwds (
  groupid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs double default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'nlhtmlsbykwds'
--

CREATE TABLE nlhtmlsbykwds (
  htmlid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs double(20,10) default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'nlkwds'
--

CREATE TABLE nlkwds (
  kwdid int(11) NOT NULL auto_increment,
  kwd varchar(50) default NULL,
  type int(11) default '1',
  PRIMARY KEY  (kwdid)
) TYPE=MyISAM;


--
-- Table structure for table 'nlkwdsbygroups'
--

CREATE TABLE nlkwdsbygroups (
  grid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0'
) TYPE=MyISAM;


--
-- Table structure for table 'nlstopkwds'
--

CREATE TABLE nlstopkwds (
  kwdid int(11) NOT NULL auto_increment,
  kwd varchar(50) NOT NULL default '',
  type int(11) default '1',
  PRIMARY KEY  (kwdid)
) TYPE=MyISAM;


--
-- Table structure for table 'nlsubprofilesbykwds'
--

CREATE TABLE nlsubprofilesbykwds (
  subprofileid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  weight float NOT NULL default '0'
) TYPE=MyISAM;


--
-- Table structure for table 'nlhistoricsubprofiles'
--

CREATE TABLE nlhistoricsubprofiles (
  historicid int(11),
  subprofileid int(11),
  kwdid int(11),
  weight float
) TYPE=MyISAM;


--
-- Table structure for table 'groups'
--

CREATE TABLE groups (
  groupid int(11) NOT NULL auto_increment,
  langid char(2) NOT NULL default '',
  PRIMARY KEY  (groupid)
) TYPE=MyISAM;


--
-- Table structure for table 'htmls'
--

CREATE TABLE htmls (
  htmlid int(11) NOT NULL auto_increment,
  html varchar(250) NOT NULL default '',
  title varchar(100) default NULL,
  langid char(2) default NULL,
  updated date default NULL,
  calculated date default NULL,
  mimetype varchar(50) default NULL,
  failed int(11) default '0',
  PRIMARY KEY  (htmlid),
  KEY index_html (html)
) TYPE=MyISAM;


--
-- Table structure for table 'htmlsbyprofiles'
--

CREATE TABLE htmlsbyprofiles (
  htmlid int(11) NOT NULL default '0',
  profileid int(11) NOT NULL default '0',
  judgement char(2) NOT NULL default '',
  when2 date default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'idealgroup'
--

CREATE TABLE idealgroup (
  profileid int(11) NOT NULL auto_increment,
  langid char(2) NOT NULL default '',
  groupid int(11) NOT NULL default '0',
  PRIMARY KEY  (profileid)
) TYPE=MyISAM;


--
-- Table structure for table 'languages'
--

CREATE TABLE languages (
  langid char(2) default NULL,
  language char(30) default NULL
) TYPE=MyISAM;


INSERT INTO languages VALUES ('en','English');
INSERT INTO languages VALUES ('fr','Fran�ais');
INSERT INTO languages VALUES ('nl','Dutch');

--
-- Table structure for table 'profiles'
--

CREATE TABLE profiles (
  profileid int(11) NOT NULL auto_increment,
  userid int(11) NOT NULL default '0',
  description varchar(100) default NULL,
  updated date default NULL,
  calculated date default NULL,
  social tinyint(1) default '0',
  topicid int(11) NOT NULL default '0',
  icon varchar(255) default NULL,
  PRIMARY KEY  (profileid)
) TYPE=MyISAM;


--
-- Table structure for table 'subprofiles'
--

CREATE TABLE subprofiles (
  subprofileid int(11) NOT NULL auto_increment,
  profileid int(11) NOT NULL default '0',
  langid char(2) NOT NULL default '',
  attached date default NULL,
  groupid int(11) default '0',
  calculated date default NULL,
  updated date default NULL,
  PRIMARY KEY  (subprofileid)
) TYPE=MyISAM;


--
-- Table structure for table 'tempchromo'
--

CREATE TABLE tempchromo (
  chromoid int(11) default NULL,
  groupid int(11) default NULL,
  lang char(2) default NULL,
  subprofileid int(11) default NULL,
  tempid tinyint(1) default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'topics'
--

CREATE TABLE topics (
  topicid int(11) NOT NULL auto_increment,
  name varchar(250) NOT NULL default '',
  parent int(11) default '0',
  langid char(2) NOT NULL default '',
  used char(1) default '0',
  PRIMARY KEY  (topicid)
) TYPE=MyISAM;

--
-- Table structure for table 'topicsbyhtmls'
--

CREATE TABLE topicsbyhtmls (
  topicid int(11) NOT NULL default '0',
  htmlid int(11) NOT NULL default '0'
) TYPE=MyISAM;


--
-- Table structure for table 'htmlsbylinks'
--

CREATE TABLE htmlsbylinks (
  htmlid int(11) NOT NULL default '0',
  linkid int(11) NOT NULL default '0',
  occurs int(11) default NULL
) TYPE=MyISAM;


--
-- Table structure for table 'historicgroups'
--

CREATE TABLE historicgroups (
  historicid int(11),
  groupid int(11),
  lang char(2),
  subprofileid int(11)
) TYPE=MyISAM;


--
-- Table structure for table 'frhistoricsubprofiles'
--

CREATE TABLE frhistoricsubprofiles (
  historicid int(11),
  subprofileid int(11),
  kwdid int(11),
  weight float
) TYPE=MyISAM;


--
-- Table structure for table 'enhistoricsubprofiles'
--

CREATE TABLE enhistoricsubprofiles (
  historicid int(11),
  subprofileid int(11),
  kwdid int(11),
  weight float
) TYPE=MyISAM;


--
-- Table structure for table 'users'
--

CREATE TABLE users (
  userid int(11) NOT NULL auto_increment,
  user varchar(20) default NULL,
  pwd varchar(20) default NULL,
  fullname varchar(50) default NULL,
  email varchar(50) default NULL,
  title varchar(50) default NULL,
  organisation varchar(100) default NULL,
  address1 varchar(100) default NULL,
  address2 varchar(100) default NULL,
  city varchar(100) default NULL,
  country varchar(50) default NULL,
  default_profile int(11) default '0',
  PRIMARY KEY  (userid)
) TYPE=MyISAM;


--
-- Table structure for table 'expertkwds'
--

CREATE TABLE expertkwds (
  PROFILEID�integer�not null,
��GROUPID�integer�not null,
��IDW_MOT�numeric(7) not null,
��MOT_EXPERT varchar(50) not null,
��LEVEL integer ,
��FREQ�varchar(128) ,
��PARENTS varchar(128)
)TYPE=MyISAM;


--
-- Table structure for table 'expertdoclist'
--

CREATE TABLE expkwdsdoclist (
  PROFILEID�integer not null,
��GROUPID integer not null,
��IDW_MOT�numeric(7) not null,
��NOM_DOC�varchar(255)
)TYPE=MyISAM;
