<?php
// navigation bar
// left.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_control_plugins.inc");

require("./left-head.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS, false, 0);
if($_pql->ldap_error) {
    session_unregister("USER_ID");
    session_unregister("USER_PASS");
    session_unregister("USER_DN");

    die("$_pql->ldap_error<br><a href=\"index.php\" target=\"_top\">Login again</a>");
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
    <?=PQL_LANG_USER; ?>: <b><?=$USER_ID?></b> |
    <a href="index.php?logout=1" target="_parent"><?=PQL_LANG_LOGOUT?></a>
  </font>
  <br>
<?php if($ADVANCED_MODE) { ?>
  <font color="black" size="-10">
    <?=$USER_DN?>
  </font>
<?php } ?>
  <form method=post action="index2.php" target="_top">
    <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit()"<?=$checked?>><u>A</u>dvanced mode
  </form>

  <!-- HOME -->
  <div id="el1Parent" class="parent">
    <a class="item" href="home.php">
      <font color="black" class="heada"><b>Home</b></font>
    </a>
  </div>

<?php if($ADVANCED_MODE) { ?>
  <font color="black" class="heada">
    Host: <font color="black" size=-4><b><?=$USER_HOST?></b></font>
  </font>
  <br>

<?php if($ALLOW_BRANCH_CREATE) { ?>
  <div id="el2Parent" class="parent">
    <nobr>
      <a href="domain_add_form.php">Add domain branch</a>
    </nobr>
  </div>

<?php
	}
}

// Get ALL domains we have access.
//	'administrator: USER_DN'
// in the domain object
foreach($_pql->ldap_basedn as $dn)  {
    $dom = pql_get_domain_value($_pql, $dn, 'administrator', $USER_DN);
    if($dom) {
	foreach($dom as $d) {
	    $domains[] = $d;
	}
    }
}

if(!isset($domains)) {
    // No domain defined -> 'ordinary' user (only show this user)
    $SINGLE_USER = 1; session_register("SINGLE_USER");

    $cn = pql_get_userattribute($_pql->ldap_linkid, $USER_DN, PQL_CONF_ATTR_CN); $cn = $cn[0];

    // Try to get the DN of the domain
    $dnparts = ldap_explode_dn($USER_DN, 0);
    for($i=1; $dnparts[$i]; $i++) {
	// Traverse the users DN backwards
	$DN = $dnparts[$i];
	for($j=$i+1; $dnparts[$j]; $j++)
	  $DN .= "," . $dnparts[$j];
	
	// Look in DN for attribute 'defaultdomain'.
	$defaultdomain = pql_get_domain_value($_pql, $DN, 'defaultdomain');
	if($defaultdomain) {
	    // A hit. This is the domain under which the user is located.
	    $domain = $DN;
	    break;
	}
    }
?>
  <!-- start domain parent -->
  <a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$USER_DN?>"><img src="images/mail_small.png" border="0" alt="<?=$cn?>"></a>&nbsp;
  <a class="item" href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$USER_DN?>"><?=$cn?></a>
  <!-- end domain parent -->

<?php
} else {
    $SINGLE_USER = 0; session_register("SINGLE_USER");

    asort($domains);
    foreach($domains as $key => $domain) {
	// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
	$d = split(',', $domain); $d = split('=', $d[0]); $d = $d[1];

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
      if($config["PQL_CONF_SHOW_USERS"][$rootdn] != 'false') {
	  // Zero out the variables, othervise we won't get users in
	  // specified domain, but also in the PREVIOUS domain shown!
	  $users = ""; $cns = "";

	  // Get all users (their DN) in this domain
	  $users = pql_get_user($_pql->ldap_linkid, $domain);
	  if(!is_array($users)){
	      // no user available for this domain
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <img src="images/mail_small.png" border="0" alt="no user defined">&nbsp;
      <a href="user_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>">no user(s)</a>
    </nobr>

    <br>

<?php
          } else {
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="user_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>">Add a user</a>
    </nobr>

    <br>

<?php
              // From the user DN, get the CN.
	      foreach ($users as $dn) {
		  $cn = pql_get_userattribute($_pql->ldap_linkid, $dn, $config["PQL_CONF_ATTR_CN"][$rootdn]);
		  $cns[$dn] = $cn[0];
	      }
              asort($cns);

	      foreach($cns as $dn => $cn) {
		  $uid   = pql_get_userattribute($_pql->ldap_linkid, $dn, $config["PQL_CONF_ATTR_UID"][$rootdn]);
		  $uid = $uid[0];

		  $uidnr = pql_get_userattribute($_pql->ldap_linkid, $dn, $config["PQL_CONF_ATTR_QMAILUID"][$rootdn]);
		  $uidnr = $uidnr[0];

		  if(($uid != 'root') or ($uidnr != '0')) {
		      // Do NOT show root user(s) here! This should (for safty's sake)
		      // not be availible to administrate through phpQLAdmin!
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$dn?>"><img src="images/mail_small.png" border="0" alt="<?=$cn?>"></a>&nbsp;
      <a class="item" href="user_detail.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$dn?>"><?=$cn?></a>
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
