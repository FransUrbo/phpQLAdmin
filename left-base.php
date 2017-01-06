<?php
// LDAP host, user, login info etc
// $Id: left-base.php,v 2.4 2007-11-20 11:50:02 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/left-head.html");
// }}}

// {{{ Find out if we're to run in ADVANCE/SIMPLE mode
if($_REQUEST["advanced"] == 1) {
  $checked  = " CHECKED";
  $_SESSION["ADVANCED_MODE"] = 1;
} else {
  $checked  = "";
  $_SESSION["ADVANCED_MODE"] = 0;
}
// }}}

// {{{ ---------------- LDAP host, user, login/logout information etc
?>
  <!-- Link to the logged in user information page -->
  <div id="el1Parent" class="parent" style="margin-bottom: 5px">
    <?php echo $LANG->_('User')?>:
    <nobr>
      <a class="item" href="user_detail.php?rootdn=<?php echo pql_get_rootdn($_SESSION["USER_DN"], 'left.php')?>&user=<?php echo urlencode($_SESSION["USER_DN"])?>">
        <span class="heada"><b><?php echo $_SESSION["USER_ID"]?></b></span>
      </a>
    </nobr>
  </div>

<?php if($_SESSION["ADVANCED_MODE"]) {
		$host = explode(';', $_SESSION["USER_HOST"]);

		// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
		$host[0] = urldecode($host[0]);
?>
  <!-- LDAP Server information -->
  <div id="el2Parent" class="parent" style="margin-bottom: 5px">
    <?php echo $LANG->_('LDAP Server')?>:
    <nobr>
      <span class="heada"><?php echo pql_maybe_idna_decode($host[0])?><?php if(!preg_match('/^ldapi:/i', $host[0])) { echo ":".$host[1]; } ?></span>
    </nobr>
  </div>

<?php } ?>

  <!-- logout link -->
  <div id="el3Parent" class="parent" style="margin-bottom: 5px">
    <nobr>
      <a class="item" href="index.php?logout=1" target="_parent">
        <span class="heada"><?php echo $LANG->_('Log out')?></span>
      </a>
    </nobr>
  </div>
<?php if(!pql_get_define("PQL_CONF_DISABLE_ADVANCED_MODE")) { ?>

  <!-- Advanced Mode selector -->
  <div id="el4Parent" class="parent">
    <form method=post action="index2.php" target="_top">
<?php	if($_SESSION["konqueror"]) { ?>
      <input type="checkbox" name="advanced" accesskey="a" onClick="this.form.submit()"<?php echo $checked?>><?php echo $LANG->_('\uA\Udvanced mode')."\n"?>
<?php	} elseif($_SESSION["lynx"]) { ?>
<?php	  if($_REQUEST["advanced"] == 1) { ?>
      <input type="hidden" name="advanced" value="0">
      <input type="submit" value="<?php echo $LANG->_('Simple mode')."\n"?>">
<?php	  } else { ?>
      <input type="hidden" name="advanced" value="1">
      <input type="submit" value="<?php echo $LANG->_('Advanced mode')."\n"?>">
<?php	  } ?>
<?php	} else { ?>
      <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit()"<?php echo $checked?>><?php echo $LANG->_('\uA\Udvanced mode')."\n"?>
<?php	}
	  }
?>
    </form>
  </div>
<?php
// }}}

// {{{ ---------------- HOME BRANCH (PROJECT URLS ETC)
// {{{ Level 1:      phpQLAdmin configuration etc
$div_counter = 10; // Initialize the global counter
pql_format_tree("<b>".$LANG->_('Home')."</b>", 'home.php');
// }}}

// {{{ Level 2[abc]: Search links, configuration and tests and setup
// Level 2a: Search links
$links = array($LANG->_('Directory search') => 'user_search.php');
pql_format_tree($LANG->_('Search'), 0, $links, 1);

if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
  // Level 2b: Configuration and tests
  $links = array($LANG->_('phpQLAdmin Configuration')	=> 'config_detail.php',
				 $LANG->_('Test LDAP-Connection')		=> 'config_ldaptest.php');
  pql_format_tree($LANG->_('phpQLAdmin Configuration'), 0, $links, 1);

  // Level 2c2: LDAP server configuration
  if($_SESSION["ACCESSLOG_OVERLAY"])
	$accesslog_overlay = array($LANG->_('LDAP Access logs')			=> 'config_ldap.php?type=accesslog&start=0');

  if($_SESSION["CONFIG_BACKEND_ENABLED"] and preg_match('/CVS/i', $_SESSION["VERSION"]))
	$config_backend = array($LANG->_('LDAP Server Configuration')	=> 'config_ldap.php?type=config');

  // Level 2c2: LDAP server setup etc
  if($_SESSION["MONITOR_BACKEND_ENABLED"]) {
	$links = array($LANG->_('LDAP Syntaxes')			=> 'config_ldap.php?type=ldapsyntaxes',
				   $LANG->_('LDAP Matching rules')		=> 'config_ldap.php?type=matchingrules',
				   $LANG->_('LDAP Attribute types')		=> 'config_ldap.php?type=attributetypes',
				   $LANG->_('LDAP Object classes')		=> 'config_ldap.php?type=objectclasses');

	if(is_array($accesslog_overlay))
	  $links = $accesslog_overlay + $links;

	if(is_array($config_backend))
	  $links = $config_backend + $links;

	pql_format_tree($LANG->_('LDAP Server Configuration'), 0, $links, 1);

	// Level 2c3: LDAP Server status
	if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
	  $links = array($LANG->_('LDAP Server Status')		=> 'status_ldap.php?type=basics',
					 $LANG->_('LDAP Connection Status')	=> 'status_ldap.php?type=connections',
					 $LANG->_('LDAP Database Status')	=> 'status_ldap.php?type=databases');
	  pql_format_tree($LANG->_('LDAP Server Statistics'), 0, $links, 1);
	}
  } elseif(is_array($config_backend) or is_array($accesslog_overlay)) {
	// No monitor backend, but config is - show only the config backend
	$links = array();

	if(is_array($accesslog_overlay))
	  $links = $accesslog_overlay + $links;

	if(is_array($config_backend))
	  $links = $config_backend + $links;

	pql_format_tree($LANG->_('LDAP Server Configuration'), 0, $links, 1);
  }
}
// }}}

// {{{ Level 2d:     Documentation etc
$links = array($LANG->_('Documentation')				=> 'doc/index.php');
if($_SESSION["ADVANCED_MODE"]) {
  $new = array($LANG->_('What\'s left todo')			=> 'TODO',
			   $LANG->_('What\'s been done')			=> 'CHANGES');
  $links = $links + $new;
  
  if($_SESSION["ALLOW_BRANCH_CREATE"]) {
	$new = array($LANG->_('Language translator')		=> 'tools/update_translations.php',
				 $LANG->_('PHP Information dump')		=> 'tools/phpinfo.php');
	$links = $links + $new;
  }
}
pql_format_tree($LANG->_('Documentation'), 0, $links, 1);
// }}}

// {{{ Level 2[ef]:  Main site and misc QmailLDAP links
if($_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
  // Level 2e: Main site and general phpQLAdmin links
  $links = array('phpQLAdmin @ Bayour'					=> 'http://phpqladmin.com/',
				 $LANG->_('Bugtracker')					=> 'http://bugs.bayour.com/');
  pql_format_tree($LANG->_('phpQLAdmin Site Specifics'), 0, $links, 1);
  
  // Level 2f: Misc QmailLDAP/Controls links
  $links = array('Official Qmail-LDAP pages'			=> 'http://www.qmail-ldap.org/',
				 'Life with Qmail-LDAP'					=> 'http://www.lifewithqmail.org/ldap/',
				 'Life with Qmail'						=> 'http://www.lifewithqmail.org/');
  pql_format_tree($LANG->_('Misc Qmail & Qmail-LDAP'), 0, $links, 1);
}
// }}}

// This an ending for the initial parent (level 0)
pql_format_tree_end();
unset($links);
// }}}

require("./left-trailer.html");

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
