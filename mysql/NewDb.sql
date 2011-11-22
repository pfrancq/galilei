-- MySQL dump 10.11
--
-- Host: localhost    Database: newsxml
-- ------------------------------------------------------
-- Server version	5.0.67-0ubuntu6

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `concepts`
--

DROP TABLE IF EXISTS `concepts`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `concepts` (
  `conceptid` int(11) unsigned NOT NULL auto_increment,
  `name` text collate utf8_bin  NOT NULL,
  `typeid` int(11) unsigned NOT NULL default '0',
  `def` text collate utf8_bin  NOT NULL,
  `refdocs` int(11) unsigned NOT NULL default '0',
  `indexdocs` int(11) unsigned NOT NULL default '0',
  `refprofiles` int(11) unsigned NOT NULL default '0',
  `indexprofiles` int(11) unsigned NOT NULL default '0',
  `refcommunities` int(11) unsigned NOT NULL default '0',
  `indexcommunities` int(11) unsigned NOT NULL default '0',
  `reftopics` int(11) unsigned NOT NULL default '0',
  `indextopics` int(11) unsigned NOT NULL default '0',
  `refclasses` int(11) unsigned NOT NULL default '0',
  `indexclasses` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`conceptid`),
  KEY `conceptndx` (`typeid`,`name`(255))
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `conceptcats`
--

DROP TABLE IF EXISTS `conceptcats`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `conceptcats` (
  `catid` int(11) unsigned NOT NULL auto_increment,
  `name` varchar(200) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`catid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `concepttypes`
--

DROP TABLE IF EXISTS `concepttypes`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `concepttypes` (
  `typeid` int(11) unsigned NOT NULL auto_increment,
  `catid` int(11) unsigned NOT NULL default '0',
  `name` varchar(200) collate utf8_bin NOT NULL,
  `description` varchar(200) collate utf8_bin NOT NULL,
  `refdocs` int(11) unsigned NOT NULL default '0',
  `refcommunities` int(11) unsigned NOT NULL default '0',
  `refprofiles` int(11) unsigned NOT NULL default '0',
  `reftopics` int(11) unsigned NOT NULL default '0',
  `refclasses` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`typeid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `statements`
--

DROP TABLE IF EXISTS `statements`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `statements` (
  `statementid` int(11) unsigned NOT NULL auto_increment,
  `predicate` text collate utf8_bin NOT NULL,
  `xi` int(11) unsigned NOT NULL,
  `xitype` int(11) unsigned NOT NULL,
  `xj` int(11) unsigned NOT NULL,
  `xjtype` int(11) unsigned NOT NULL,
  `weight` double NOT NULL default '0.0000000000',
  PRIMARY KEY  (`statementid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `predicates`
--

DROP TABLE IF EXISTS `predicates`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `predicates` (
  `predicateid` int(11) unsigned NOT NULL auto_increment,
  `name` varchar(50) collate utf8_bin NOT NULL,
  `description` varchar(200) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`predicateid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `classes`
--

DROP TABLE IF EXISTS `classes`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `classes` (
  `classid` int(11) unsigned NOT NULL auto_increment,
  `name` varchar(250) collate utf8_bin NOT NULL default '',
  `parent` int(11) unsigned default '0',
  `blockid` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`classid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `groups`
--

DROP TABLE IF EXISTS `communities`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `communities` (
  `communityid` int(11) unsigned NOT NULL auto_increment,
  `langid` char(2) NOT NULL,
  `updated` date NOT NULL,
  `calculated` date NOT NULL,
  `blockid` int(11) unsigned NOT NULL default '0',
  `name` varchar(250) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`communityid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `docs`
--

DROP TABLE IF EXISTS `docs`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `docs` (
  `docid` int(11) unsigned NOT NULL auto_increment,
  `doc` varchar(250) collate utf8_bin NOT NULL,
  `title` text collate utf8_bin NOT NULL,
  `langid` char(2),
  `updated` date NOT NULL,
  `calculated` date NOT NULL,
  `mimetype` varchar(50) collate utf8_bin NOT NULL,
  `failed` int(11) unsigned default '0',
  `blockid` int(11) unsigned NOT NULL default '0',
  `structid` int(11) unsigned NOT NULL default '0',
  `topicid` int(11) unsigned NOT NULL default '0',
  `attached` date NOT NULL,
  PRIMARY KEY  (`docid`),
  KEY `index_doc` (`doc`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `docsbygroups`
--

DROP TABLE IF EXISTS `docsbycommunities`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `docsbycommunities` (
  `docid` int(11) unsigned NOT NULL,
  `communityid` int(11) unsigned NOT NULL,
  `ranking` double NOT NULL default '0.0000000000'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `docsbylinks`
--

DROP TABLE IF EXISTS `docsbylinks`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `docsbylinks` (
  `docid` int(11) unsigned NOT NULL,
  `linkid` int(11) unsigned NOT NULL,
  `occurs` int(11) unsigned default NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `docsbyprofiles`
--

DROP TABLE IF EXISTS `docsbyprofiles`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `docsbyprofiles` (
  `docid` int(11) unsigned NOT NULL,
  `profileid` int(11) unsigned NOT NULL,
  `fdbk` int(3) NOT NULL,
  `done` date NOT NULL,
  `langid` char(2) NOT NULL,
  `computed` date NOT NULL,
  `updated` date NOT NULL,
  KEY `profileidx` (`profileid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `profiles`
--

DROP TABLE IF EXISTS `profiles`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `profiles` (
  `profileid` int(11) unsigned NOT NULL auto_increment,
  `userid` int(11) unsigned NOT NULL,
  `profiletype` int(3) unsigned NOT NULL,
  `description` varchar(100) collate utf8_bin NOT NULL,
  `social` tinyint(1) NOT NULL default '1',
  `subjectid` int(11) unsigned NOT NULL default '0',
  `attached` date NOT NULL,
  `communityid` int(11) unsigned NOT NULL default '0',
  `calculated` date default NULL,
  `blockid` int(11) unsigned NOT NULL default '0',
  `updated` date NOT NULL,
  `level` tinyint(3) NOT NULL default '0',
  `score` double NOT NULL default '0.0000000000',
  PRIMARY KEY  (`profileid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `subjects`
--

DROP TABLE IF EXISTS `subjects`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `subjects` (
  `subjectid` int(11) unsigned NOT NULL,
  `name` varchar(250) collate utf8_bin NOT NULL,
  `parent` int(11) unsigned NOT NULL,
  `used` BOOL default '1',
  PRIMARY KEY  (`subjectid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `subjectsbydocs`
--

DROP TABLE IF EXISTS `subjectsbydocs`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `subjectsbydocs` (
  `subjectid` int(11) unsigned NOT NULL,
  `docid` int(11) unsigned NOT NULL,
  `used` int(1) unsigned NOT NULL,
   PRIMARY KEY  (`subjectid`,`docid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `sugsbygroups`
--

DROP TABLE IF EXISTS `sugsbycommunities`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `sugsbycommunities` (
  `communityid` int(11) unsigned NOT NULL,
  `docid` int(11) unsigned NOT NULL,
  `ranking` double NOT NULL default '0.0000000000',
  `proposed` date NOT NULL,
  `info` text collate utf8_bin
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `sugsbyprofiles`
--

DROP TABLE IF EXISTS `sugsbyprofiles`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `sugsbyprofiles` (
  `profileid` int(11) unsigned NOT NULL,
  `docid` int(11) unsigned NOT NULL,
  `ranking` double NOT NULL default '0.0000000000',
  `proposed` date NOT NULL,
  `info` text collate utf8_bin
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `topics`
--

DROP TABLE IF EXISTS `topics`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `topics` (
  `topicid` int(10) unsigned NOT NULL auto_increment,
  `updated` date NOT NULL,
  `calculated` date NOT NULL,
  `blockid` int(11) unsigned NOT NULL default '0',
  `name` varchar(250) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`topicid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `users` (
  `userid` int(11) NOT NULL auto_increment,
  `user` varchar(20) collate utf8_bin NOT NULL,
  `fullname` varchar(50) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`userid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2009-04-10  8:08:01
