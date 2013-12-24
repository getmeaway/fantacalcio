-- phpMyAdmin SQL Dump
-- version 2.11.11.3
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generato il: 24 Dic, 2013 at 01:18 PM
-- Versione MySQL: 5.1.58
-- Versione PHP: 4.4.9

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `my_fantacazzismo`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_competitions`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_competitions` (
  `c_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) NOT NULL DEFAULT '',
  `active` tinyint(4) NOT NULL DEFAULT '0',
  `matches` tinyint(11) NOT NULL DEFAULT '0',
  `standings` tinyint(11) NOT NULL DEFAULT '0',
  `lineups` tinyint(11) NOT NULL DEFAULT '0',
  `newsletter` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`c_id`),
  KEY `active` (`active`),
  KEY `matches` (`matches`),
  KEY `lineups` (`lineups`),
  KEY `newsletter` (`newsletter`),
  KEY `standings` (`standings`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_groups`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_groups` (
  `g_id` int(11) NOT NULL AUTO_INCREMENT,
  `c_id` int(11) NOT NULL DEFAULT '0',
  `name` varchar(64) NOT NULL DEFAULT '',
  `active` int(11) NOT NULL DEFAULT '0',
  `standings` tinyint(4) NOT NULL DEFAULT '0',
  `matches` int(11) NOT NULL DEFAULT '0',
  `lineups` int(1) NOT NULL DEFAULT '0',
  `newsletter_standings` tinyint(4) NOT NULL DEFAULT '0',
  `newsletter_matches` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`g_id`),
  KEY `c_id` (`c_id`),
  KEY `active` (`active`),
  KEY `standings` (`standings`),
  KEY `matches` (`matches`),
  KEY `lineups` (`lineups`),
  KEY `newsletter_standings` (`newsletter_standings`),
  KEY `newsletter_matches` (`newsletter_matches`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=11 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_lineups`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_lineups` (
  `f_id` int(11) NOT NULL AUTO_INCREMENT,
  `pl_id` int(11) NOT NULL DEFAULT '0',
  `t_id` int(11) NOT NULL DEFAULT '0',
  `c_id` int(11) NOT NULL DEFAULT '0',
  `round` int(11) NOT NULL DEFAULT '0',
  `position` int(11) NOT NULL DEFAULT '0',
  `has_played` int(11) NOT NULL DEFAULT '0',
  `penalty_position` int(11) NOT NULL DEFAULT '0',
  `timestamp` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  PRIMARY KEY (`f_id`),
  KEY `pl_id` (`pl_id`),
  KEY `t_id` (`t_id`),
  KEY `c_id` (`c_id`),
  KEY `round` (`round`),
  KEY `position` (`position`),
  KEY `has_played` (`has_played`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=12691 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_lineups_inserts`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_lineups_inserts` (
  `i_id` int(11) NOT NULL AUTO_INCREMENT,
  `t_id` int(11) NOT NULL,
  `c_id` int(11) NOT NULL,
  `round` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  `timestamp` int(11) NOT NULL,
  PRIMARY KEY (`i_id`),
  KEY `t_id` (`t_id`),
  KEY `c_id` (`c_id`),
  KEY `round` (`round`),
  KEY `status` (`status`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=717 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_matches`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_matches` (
  `m_id` int(11) NOT NULL AUTO_INCREMENT,
  `g_id` int(11) NOT NULL DEFAULT '0',
  `t1_id` int(11) NOT NULL DEFAULT '0',
  `t2_id` int(11) NOT NULL DEFAULT '0',
  `pt_1` float NOT NULL DEFAULT '0',
  `mod_por_1` float NOT NULL DEFAULT '0',
  `mod_dif_1` float NOT NULL DEFAULT '0',
  `mod_centr_1` float NOT NULL DEFAULT '0',
  `mod_att_1` float NOT NULL DEFAULT '0',
  `tot_1` float NOT NULL DEFAULT '0',
  `goals_1` int(11) NOT NULL DEFAULT '0',
  `pt_ot_1` float NOT NULL,
  `goals_ot_1` int(11) NOT NULL DEFAULT '0',
  `penalties_1` int(11) NOT NULL DEFAULT '0',
  `pt_2` float NOT NULL DEFAULT '0',
  `mod_por_2` float NOT NULL DEFAULT '0',
  `mod_dif_2` float NOT NULL DEFAULT '0',
  `mod_centr_2` float NOT NULL DEFAULT '0',
  `mod_att_2` float NOT NULL DEFAULT '0',
  `tot_2` float NOT NULL DEFAULT '0',
  `goals_2` int(11) NOT NULL DEFAULT '0',
  `pt_ot_2` float NOT NULL,
  `goals_ot_2` int(11) NOT NULL DEFAULT '0',
  `penalties_2` int(11) NOT NULL DEFAULT '0',
  `round` int(11) NOT NULL DEFAULT '0',
  `played` tinyint(4) NOT NULL DEFAULT '0',
  `winner_id` int(11) NOT NULL DEFAULT '0',
  `bonus_t1` int(11) NOT NULL DEFAULT '0',
  `bonus_t2` int(11) NOT NULL DEFAULT '0',
  `match_label` char(2) DEFAULT NULL,
  `t1_label` varchar(20) NOT NULL DEFAULT '',
  `t2_label` varchar(20) NOT NULL DEFAULT '',
  PRIMARY KEY (`m_id`),
  KEY `g_id` (`g_id`),
  KEY `t1_id` (`t1_id`),
  KEY `t2_id` (`t2_id`),
  KEY `round` (`round`),
  KEY `played` (`played`),
  KEY `winner_id` (`winner_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=707 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_payments`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_payments` (
  `t_id` int(11) NOT NULL,
  `initial_quota` int(11) NOT NULL,
  `initial_quota_fine` bit(1) NOT NULL DEFAULT b'0',
  `payed` int(11) NOT NULL,
  `win_quota` int(11) NOT NULL,
  `lost_quota` int(11) NOT NULL,
  PRIMARY KEY (`t_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_penalties`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_penalties` (
  `penalty_id` int(11) NOT NULL AUTO_INCREMENT,
  `pl_id` int(11) NOT NULL,
  `c_id` int(11) NOT NULL,
  `round` int(11) NOT NULL,
  `goal` tinyint(4) NOT NULL,
  PRIMARY KEY (`penalty_id`),
  KEY `pl_id` (`pl_id`),
  KEY `c_id` (`c_id`),
  KEY `round` (`round`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_players`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_players` (
  `pl_id` int(11) NOT NULL DEFAULT '0',
  `name` varchar(64) NOT NULL DEFAULT '',
  `role` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`pl_id`),
  KEY `role` (`role`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_players_status`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_players_status` (
  `s_id` int(11) NOT NULL AUTO_INCREMENT,
  `pl_id` int(11) NOT NULL,
  `round` int(11) NOT NULL,
  `status` varchar(32) NOT NULL,
  `note` varchar(255) NOT NULL,
  PRIMARY KEY (`s_id`),
  KEY `pl_id` (`pl_id`,`round`),
  KEY `round` (`round`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_players_teams`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_players_teams` (
  `pt_id` int(11) NOT NULL AUTO_INCREMENT,
  `pl_id` int(11) NOT NULL,
  `round` int(11) NOT NULL,
  `rt_id` int(11) NOT NULL,
  `active` tinyint(4) NOT NULL,
  PRIMARY KEY (`pt_id`),
  KEY `pl_id` (`pl_id`),
  KEY `round` (`round`),
  KEY `rt_id` (`rt_id`),
  KEY `active` (`active`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=24054 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_real_teams`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_real_teams` (
  `rt_id` int(11) NOT NULL AUTO_INCREMENT,
  `long_name` varchar(64) NOT NULL,
  `short_name` varchar(8) NOT NULL,
  `active` bit(1) NOT NULL,
  PRIMARY KEY (`rt_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=21 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_real_teams_matches`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_real_teams_matches` (
  `m_id` int(11) NOT NULL AUTO_INCREMENT,
  `round` int(11) NOT NULL,
  `rt1_id` int(11) NOT NULL,
  `rt2_id` int(11) NOT NULL,
  `active` tinyint(4) NOT NULL,
  PRIMARY KEY (`m_id`),
  KEY `rt1_id` (`rt1_id`,`rt2_id`),
  KEY `round` (`round`),
  KEY `rt2_id` (`rt2_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=512 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_rounds`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_rounds` (
  `round` int(11) NOT NULL DEFAULT '0',
  `date` int(11) NOT NULL DEFAULT '0',
  `end_date` int(11) NOT NULL,
  `status` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`round`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_rounds_competitions`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_rounds_competitions` (
  `rc_id` int(11) NOT NULL AUTO_INCREMENT,
  `round` int(11) NOT NULL,
  `c_id` int(11) NOT NULL,
  `competition_round` int(11) NOT NULL,
  `round_label` varchar(40) NOT NULL,
  `next` tinyint(4) NOT NULL,
  PRIMARY KEY (`rc_id`),
  KEY `round` (`round`),
  KEY `c_id` (`c_id`),
  KEY `competition_round` (`competition_round`),
  KEY `next` (`next`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=327 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_round_statuses`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_round_statuses` (
  `s_id` int(11) NOT NULL,
  `status` varchar(32) NOT NULL,
  `result` tinyint(4) NOT NULL,
  PRIMARY KEY (`s_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_sfide`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_sfide` (
  `sid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `m_id` int(10) unsigned NOT NULL DEFAULT '0',
  `t1_id` int(10) unsigned NOT NULL DEFAULT '0',
  `t2_id` int(10) unsigned NOT NULL DEFAULT '0',
  `bet` varchar(128) NOT NULL DEFAULT '',
  `status` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`sid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_squads`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_squads` (
  `r_id` int(11) NOT NULL AUTO_INCREMENT,
  `t_id` int(11) NOT NULL DEFAULT '0',
  `pl_id` int(11) NOT NULL DEFAULT '0',
  `cost` int(11) NOT NULL DEFAULT '0',
  `in_team` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`r_id`),
  KEY `t_id` (`t_id`),
  KEY `pl_id` (`pl_id`),
  KEY `in_team` (`in_team`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=640 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_squad_changes`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_squad_changes` (
  `s_id` int(11) NOT NULL AUTO_INCREMENT,
  `t1_id` int(11) NOT NULL DEFAULT '0',
  `t2_id` int(11) NOT NULL DEFAULT '0',
  `money` int(11) NOT NULL DEFAULT '0',
  `active` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`s_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_teams`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_teams` (
  `t_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) NOT NULL DEFAULT '',
  `coach` varchar(64) NOT NULL DEFAULT '',
  `stadium` varchar(64) NOT NULL DEFAULT '',
  `shirt` varchar(64) NOT NULL DEFAULT '',
  `active` tinyint(4) NOT NULL DEFAULT '0',
  `uid` int(11) NOT NULL DEFAULT '0',
  `last_year` varchar(255) NOT NULL,
  `history` text NOT NULL,
  PRIMARY KEY (`t_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=29 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_teams_groups`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_teams_groups` (
  `tg_id` int(11) NOT NULL AUTO_INCREMENT,
  `g_id` int(11) NOT NULL DEFAULT '0',
  `t_id` int(11) NOT NULL DEFAULT '0',
  `active` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`tg_id`),
  KEY `g_id` (`g_id`),
  KEY `t_id` (`t_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=739 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_votes`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_votes` (
  `v_id` int(11) NOT NULL AUTO_INCREMENT,
  `pl_id` int(11) NOT NULL DEFAULT '0',
  `round` int(11) NOT NULL DEFAULT '0',
  `provider` int(11) NOT NULL,
  `total` float(3,1) NOT NULL DEFAULT '0.0',
  `vote` float(3,1) NOT NULL DEFAULT '0.0',
  `goals_for` int(11) NOT NULL DEFAULT '0',
  `penalty_goals` int(11) NOT NULL DEFAULT '0',
  `goals_against` int(11) NOT NULL DEFAULT '0',
  `assists` int(11) NOT NULL DEFAULT '0',
  `yellow_cards` int(11) NOT NULL DEFAULT '0',
  `red_cards` int(11) NOT NULL DEFAULT '0',
  `saved_penalties` int(11) NOT NULL DEFAULT '0',
  `missed_penalties` int(11) NOT NULL DEFAULT '0',
  `own_goals` int(11) NOT NULL DEFAULT '0',
  `draw_goals` int(11) NOT NULL DEFAULT '0',
  `win_goals` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`v_id`),
  KEY `pl_id` (`pl_id`),
  KEY `round` (`round`),
  KEY `provider` (`provider`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=24993 ;

-- --------------------------------------------------------

--
-- Struttura della tabella `drupal_fanta_vote_providers`
--

CREATE TABLE IF NOT EXISTS `drupal_fanta_vote_providers` (
  `p_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) NOT NULL,
  `active` tinyint(4) NOT NULL,
  PRIMARY KEY (`p_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;
