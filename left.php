<?php
// navigation bar
// left.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql_config.inc");
require("./left-head.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS, false, 0);
if($_pql->ldap_error) {
    session_unregister("USER_ID");
    session_unregister("USER_PASS");
    session_unregister("USER_DN");

    die("$_pql->ldap_error<br><a href=\"index.php\" target=\"_top\">".$LANG->_('Login again')."</a>");
}

// find out if we're to run in ADVANCE/SIMPLE mode
if($advanced == 1) {
    $checked  = " CHECKED";
    $ADVANCED_MODE = 1;

    session_register("ADVANCED_MODE");
} else {
    $checked  = "";
    $ADVANCED_MODE = 0;

    session_register("ADVANCED_MODE");
}
?>
  <font color="black" class="heada">
    <?=$LANG->_('User')?>: <b><a href="user_detail.php?user=<?=$USER_DN?>"><?=$USER_ID?></a></b>
  </font>
  <br>
<?php if($ADVANCED_MODE) {
	$host = split(';', $USER_HOST);
?>

  <font color="black" class="heada"><?=$LANG->_('Host')?>: <b><?=$host[0]?>:<?=$host[1]?></b></font>
  <br>
<?php } ?>

  <font color="black" class="heada">
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="index.php?logout=1" target="_parent"><?=$LANG->_('Log out')?></a>
  </font>

  <br>

  <form method=post action="index2.php" target="_top">
    <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit()"<?=$checked?>><?=$LANG->_('\uA\Udvanced mode')?>
  </form>
<?php if($ALLOW_BRANCH_CREATE and $ADVANCED_MODE) { ?>

  <div id="el2Parent" class="parent">
    <nobr><a href="domain_add_form.php?rootdn=<?=$_pql->ldap_basedn[0]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain branch'))); ?></a></nobr>
  </div>
<?php  } ?>

  <!-- HOME -->
  <div id="el1Parent" class="parent">
    <a class="item" href="home.php" onClick="if (capable) {expandBase('el1', true); return false;}">
      <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el1Img">
    </a>

    <a class="item" href="home.php">
      <font color="black" class="heada"><b><?=$LANG->_('Home')?></b></font>
    </a>
  </div>

  <div id="el1Child" class="child">
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="user_search.php"><?=$LANG->_('Find user')?></a></nobr><br>
<?php if($ADVANCED_MODE) { ?>
<?php	if($ALLOW_BRANCH_CREATE) { ?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="config_detail.php"><?=$LANG->_('Show configuration')?></a></nobr><br>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="config_ldaptest.php"><?=$LANG->_('Test LDAP-Connection')?></a></nobr><br>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="config_ldap.php"><?=$LANG->_('LDAP server configuration')?></a></nobr><br>
<?php	} ?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="doc/index.php"><?=$LANG->_('Documentation')?></a></nobr><br>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="TODO"><?=$LANG->_('What\'s left todo')?></a></nobr><br>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="CHANGES"><?=$LANG->_('What\'s been done')?></a></nobr><br>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="update_translations.php" target="_new"><?=$LANG->_('Language translator')?></a></nobr><br>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://phpqladmin.bayour.com/">phpqladmin.bayour.com</a></nobr><br>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://papadoc.bayour.com:23000/anthill/" target="_new"><?=$LANG->_('Bugtracker')?></a></nobr><br>
<?php } ?>
  </div>
  <!-- HOME -->

<?php
// Get ALL domains we have access.
//	'administrator: USER_DN'
// in the domain object
foreach($_pql->ldap_basedn as $dn)  {
    $dom = pql_get_domain_value($_pql, $dn, pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"), $USER_DN);
    if($dom) {
	foreach($dom as $d) {
	    $domains[] = urlencode($d);
	}
    }
}

if(!isset($domains)) {
    // No domain defined -> 'ordinary' user (only show this user)
    $SINGLE_USER = 1; session_register("SINGLE_USER");

    $cn = pql_get_userattribute($_pql->ldap_linkid, $USER_DN, pql_get_define("PQL_GLOB_ATTR_CN")); $cn = $cn[0];

    // Try to get the DN of the domain
    $dnparts = ldap_explode_dn($USER_DN, 0);
    for($i=1; $dnparts[$i]; $i++) {
	// Traverse the users DN backwards
	$DN = $dnparts[$i];
	for($j=$i+1; $dnparts[$j]; $j++)
	  $DN .= "," . $dnparts[$j];
	
	// Look in DN for attribute 'defaultdomain'.
	$defaultdomain = pql_get_domain_value($_pql, $DN, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));
	if($defaultdomain) {
	    // A hit. This is the domain under which the user is located.
	    $domain = $DN;
	    break;
	}
    }
?>
  <!-- start domain parent -->
  <a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?php echo urlencode($USER_DN); ?>"><img src="images/mail_small.png" border="0" alt="<?=$cn?>"></a>&nbsp;
  <a class="item" href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?php echo urlencode($USER_DN); ?>"><?=$cn?></a>
  <!-- end domain parent -->

<?php
} else {
    $SINGLE_USER = 0; session_register("SINGLE_USER");

    asort($domains);
    foreach($domains as $key => $domain) {
	// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
	$d = split(',', urldecode($domain)); $d = split('=', $d[0]); $d = $d[1];

	// Get Root DN
	$rootdn = pql_get_rootdn($domain);

	$j = $key + 2;
?>
  <!-- start domain parent -->
  <div id="el<?=$j?>Parent" class="parent">
    <a class="item" href="domain_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
      <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
    </a>

    <a class="item" href="domain_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>">
      <font color="black" class="heada"><?=$d?></font>
    </a>
  </div>
  <!-- end domain parent -->

  <!-- start domain children -->
  <div id="el<?=$j?>Child" class="child">
<?php
      // iterate trough all users
      if(pql_get_define("PQL_CONF_SHOW_USERS", $rootdn)) {
	  // Zero out the variables, othervise we won't get users in
	  // specified domain, but also in the PREVIOUS domain shown!
	  $users = ""; $cns = "";

	  // Get all users (their DN) in this domain
	  $users = pql_get_user($_pql->ldap_linkid, $domain);
	  if(!is_array($users)) {
	      // no user available for this domain
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <img src="images/mail_small.png" border="0" alt="<?=$LANG->_('No users defined')?>">&nbsp;
      <a href="user_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><?=$LANG->_('No users defined')?></a>
    </nobr>

    <br>

<?php	  } else { ?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="user_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('user'))); ?></a>
    </nobr>

    <br>

<?php         // From the user DN, get the CN.
	      foreach ($users as $dn) {
		  unset($cn); unset($sn);

		  $cn = pql_get_userattribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_GIVENNAME"));
		  $sn = pql_get_userattribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_SN"));

		  if($cn[0] && $sn[0])
		    // Only remember users that have both a first and lastname.
		    $cns[$dn] = $sn[0].", ".$cn[0];
		  else {
		      $cn = pql_get_userattribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_CN"));
		      $cns[$dn] = "System - ".$cn[0];
		  }
	      }
              asort($cns);

	      foreach($cns as $dn => $cn) {
		  $uid = pql_get_userattribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_UID"));
		  $uid = $uid[0];

		  $uidnr = pql_get_userattribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_GLOB_ATTR_QMAILUID"));
		  $uidnr = $uidnr[0];

		  if(($uid != 'root') or ($uidnr != '0')) {
		      // Do NOT show root user(s) here! This should (for safty's sake)
		      // not be availible to administrate through phpQLAdmin!
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?php echo urlencode($dn); ?>"><img src="images/mail_small.png" border="0" alt="<?=$cn?>"></a>&nbsp;
      <a class="item" href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?php echo urlencode($dn); ?>"><?=$cn?></a>
    </nobr>

    <br>

<?php
		  }
              }
          }
      }
?>
  </div>
  <!-- end domain children -->

<?php
    } // end foreach ($domains)
} // end if(is_array($domains))

require("./left-trailer.html");
?>
