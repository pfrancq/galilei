
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
-- Table structure for table 'bookmarks'
--

CREATE TABLE bookmarks (
  bookmarkid int(11) NOT NULL,
  profileid int(11) NOT NULL,
  bookmark varchar(100) default NULL,
  parentid int(11) DEFAULT '0',
  icon VARCHAR(255) DEFAULT NULL,
  pos int(11),
  PRIMARY KEY (bookmarkid,profileid)
) TYPE=MyISAM CHARACTER SET utf8;


--
-- Table structure for table 'bookmarksbyhtmls'
--

CREATE TABLE bookmarksbyhtmls (
  bookmarkid int(11) NOT NULL,
  profileid int(11) NOT NULL,
  title varchar(100) DEFAULT NULL,
  htmlid int(11) NOT NULL,
  icon VARCHAR(255) DEFAULT NULL,
  pos int(11),
  KEY index_bookmarkid(bookmarkid,profileid)
) TYPE=MyISAM CHARACTER SET utf8;

#
# Table structure for table kwds
#


CREATE TABLE kwds (
  kwdautoid int(11) NOT NULL auto_increment,
  kwdid int(11) NOT NULL default '0',
  kwd varchar(50) default NULL,
  type int(11) default '1',
  langid char(2) default NULL,
  INDEX langidndx (langid),
  INDEX langidsndx (langid,kwdid),
  PRIMARY KEY  (kwdautoid)
) TYPE=MyISAM CHARACTER SET utf8 AUTO_INCREMENT=1 ;


--
-- Table structure for table 'groups'
--

CREATE TABLE groups (
  groupid int(11) NOT NULL auto_increment,
  langid char(2) NOT NULL default '',
  PRIMARY KEY  (groupid)
) TYPE=MyISAM CHARACTER SET utf8;

#
# Table structure for table `groupsbykwds`
#

CREATE TABLE groupsbykwds (
  groupid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs double default NULL,
  langid char(2) default NULL,
  INDEX langidndx (langid)
) TYPE=MyISAM CHARACTER SET utf8;

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
  owner int(11) default '0',
  PRIMARY KEY  (htmlid),
  KEY index_html (html)
) TYPE=MyISAM CHARACTER SET utf8;


--
-- Table structure for table 'htmlsbyprofiles'
--

CREATE TABLE htmlsbyprofiles (
  htmlid int(11) NOT NULL default '0',
  profileid int(11) NOT NULL default '0',
  judgement char(2) NOT NULL default '',
  when2 date default NULL
) TYPE=MyISAM CHARACTER SET utf8;


#
# Table structure for table `htmlsbykwds`
#

CREATE TABLE htmlsbykwds (
  htmlid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  occurs double(20,10) default NULL,
  langid char(2) default NULL,
  INDEX langidndx (langid)
) TYPE=MyISAM CHARACTER SET utf8;

#
# Table structure for table `kwdsbygroups`
#

CREATE TABLE kwdsbygroups (
  grid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  langid char(2) default NULL,
  INDEX langidndx (langid)
) TYPE=MyISAM CHARACTER SET utf8;

#
# Table structure for table `stopkwds`
#

CREATE TABLE stopkwds (
  kwdautoid int(11) NOT NULL auto_increment,
  kwdid int(11) NOT NULL default '0',
  kwd varchar(50) NOT NULL default '',
  type int(11) default '1',
  langid char(2) default NULL,
  INDEX langidndx (langid),
  PRIMARY KEY  (kwdautoid)
) TYPE=MyISAM CHARACTER SET utf8 AUTO_INCREMENT=1 ;

#
# Table structure for table `subprofilesbykwds`
#

CREATE TABLE subprofilesbykwds (
  subprofileid int(11) NOT NULL default '0',
  kwdid int(11) NOT NULL default '0',
  weight float NOT NULL default '0',
  langid char(2) default NULL,
  INDEX langidndx (langid)
) TYPE=MyISAM CHARACTER SET utf8;

--
-- Table structure for table 'idealgroup'
--

CREATE TABLE idealgroup (
  profileid int(11) NOT NULL auto_increment,
  langid char(2) NOT NULL default '',
  groupid int(11) NOT NULL default '0',
  PRIMARY KEY  (profileid)
) TYPE=MyISAM CHARACTER SET utf8;

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
) TYPE=MyISAM CHARACTER SET utf8;


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
) TYPE=MyISAM CHARACTER SET utf8;


--
-- Table structure for table 'tempchromo'
--

CREATE TABLE tempchromo (
  chromoid int(11) default NULL,
  groupid int(11) default NULL,
  lang char(2) default NULL,
  subprofileid int(11) default NULL,
  tempid tinyint(1) default NULL
) TYPE=MyISAM CHARACTER SET utf8;


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
) TYPE=MyISAM CHARACTER SET utf8;

--
-- Table structure for table 'topicsbyhtmls'
--

CREATE TABLE topicsbyhtmls (
  topicid int(11) NOT NULL default '0',
  htmlid int(11) NOT NULL default '0'
) TYPE=MyISAM CHARACTER SET utf8;


--
-- Table structure for table 'htmlsbylinks'
--

CREATE TABLE htmlsbylinks (
  htmlid int(11) NOT NULL default '0',
  linkid int(11) NOT NULL default '0',
  occurs int(11) default NULL
) TYPE=MyISAM CHARACTER SET utf8;


--
-- Table structure for table 'historicgroups'
--

CREATE TABLE historicgroups (
  historicid int(11),
  date Date DEFAULT '0000-00-00',
  groupid int(11),
  lang char(2),
  subprofileid int(11)
) TYPE=MyISAM CHARACTER SET utf8;


#
# Table structure for table `historicsubprofiles`
#

CREATE TABLE historicsubprofiles (
  historicid int(11) default NULL,
  subprofileid int(11) default NULL,
  kwdid int(11) default NULL,
  weight float default NULL,
  date date default '0000-00-00',
  langid char(2) default NULL,
  INDEX langidndx (langid)
) TYPE=MyISAM CHARACTER SET utf8;


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
) TYPE=MyISAM CHARACTER SET utf8;


--
-- Table structure for table 'expertkwds'
--

CREATE TABLE expertkwds (
  profileid integer not null,
  groupid integer not null,
  idw_mot numeric(7) not null,
  mot_expert varchar(50) not null,
  level integer ,
  freq varchar(128) ,
  parents varchar(128)
)TYPE=MyISAM CHARACTER SET utf8;


--
-- Table structure for table 'expertdoclist'
--

CREATE TABLE expkwdsdoclist (
  profileid integer not null,
  groupid integer not null,
  idw_mot numeric(7) not null,
  nom_doc varchar(255)
)TYPE=MyISAM CHARACTER SET utf8;


-- 
-- Creation of the indexes: 
--

create index idx_htmls_kwds on htmlsbykwds(htmlid,kwdid);
create index idx_subprofiles_kwds on subprofilesbykwds(subprofileid,kwdid);
create index idx_groups_kwds on groupsbykwds(groupid,kwdid);
create index idx_lang on stopkwds(langid);
create index idx_lang on kwds(langid);
