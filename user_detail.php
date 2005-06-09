<?php
// shows details of a user
// $Id: user_detail.php,v 2.91 2005-06-09 15:17:47 turbo Exp $
//
// {{{ Setup session
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

if(!$_GET and ($_REQUEST["view"] == "antispam")) {
	// We're coming from "./tables/user_details-antispam.inc",
	// so the _GET array isn't availible (which it is if we're
	// comming from any of the view buttons or the left frame),
	// but _REQUEST is (we're posting...). Make _GET and _REQUEST
	// work seemingless...
    $_GET = $_REQUEST;

	if($_REQUEST["commit"]) {
		// We're committing the SpamAssassin configuration
		// -> save changes (call an include which have all
		//    the logic).
		require($_SESSION["path"]."/include/attrib.spamassassin.inc");
		attribute_save($_REQUEST);
	}
}

// Check if user exists
if(!pql_get_dn($_pql->ldap_linkid, $_GET["user"], '(objectclass=*)', 'BASE')) {
    echo pql_complete_constant($LANG->_('User %user% does not exist'), array('user' => '<u>'.$_GET["user"].'</u>'));
    exit();
}

if(!$_GET["rootdn"]) {
	$_GET["rootdn"] = pql_get_rootdn($_GET["user"], 'user_detail.php');
}

$url["domain"] = pql_format_urls($_GET["domain"]);
$url["user"]   = pql_format_urls($_GET["user"]);
$url["rootdn"] = pql_format_urls($_GET["rootdn"]);

// Get default domain name for this domain
if($_GET["domain"]) {
	$defaultdomain = pql_get_attribute($_pql->ldap_linkid, $url["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
}

include($_SESSION["path"]."/header.html");
// }}}

// {{{ Print status message, if one is available
if(isset($_GET["msg"])) {
	pql_format_status_msg($_GET["msg"]);
}
// }}}

// {{{ Reload navigation bar if needed
if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
	if($_REQUEST["rlnb"] == 1) {
?>
  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    // This doesn't work as it's supposed to... Don't know enough java to figure it out either...
    //refreshFrames();

    // This work perfectly though...
    parent.frames.pqlnav.location.reload();
    parent.frames.pqlnavctrl.location.reload();
    parent.frames.pqlnavezmlm.location.reload();
  //--></script>
<?php
	} elseif($_REQUEST["rlnb"] == 2) {
?>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    parent.frames.pqlnav.location.reload();
  //--></script>
<?php   }
}
// }}}

// {{{ Retreive and show username etc
$username = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_CN"));
if(!$username) {
    // No common name, use uid field
    $username = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_UID"));
}
if($username and is_array($username)) {
	$username = $username[0];
}
?>

  <span class="title1"><?=$username?></span>
  <br><br>
<?php
// }}}
if(empty($_GET["view"]))
	$_GET["view"] = 'basic';

/* DLW: Setting all of these variables in the main code is pointless.  Most of them
 *      never get used.  They should be handled by the tables
 *      that actually request the variables (user_details-mailbox.inc). */

// {{{ Get basic user information
// Some of these (everything after the 'homedirectory')
// uses 'objectClass: pilotPerson' -> http://rfc-1274.rfcindex.net/
$attribs = array("cn"					=> pql_get_define("PQL_ATTR_CN"),
				 "sn"					=> pql_get_define("PQL_ATTR_SN"),
				 "uidnumber"			=> pql_get_define("PQL_ATTR_QMAILUID"),
				 "gidnumber"			=> pql_get_define("PQL_ATTR_QMAILGID"),
				 "loginshell"			=> pql_get_define("PQL_ATTR_LOGINSHELL"),
				 "uid"					=> pql_get_define("PQL_ATTR_UID"),
				 "userpassword"			=> pql_get_define("PQL_ATTR_PASSWD"),
				 "mailmessagestore"		=> pql_get_define("PQL_ATTR_MAILSTORE"),
				 "mail"					=> pql_get_define("PQL_ATTR_MAIL"),
				 "mailhost"				=> pql_get_define("PQL_ATTR_MAILHOST"),
				 "homedirectory"		=> pql_get_define("PQL_ATTR_HOMEDIR"),
				 "roomnumber"			=> pql_get_define("PQL_ATTR_ROOMNUMBER"),
				 "telephonenumber"		=> pql_get_define("PQL_ATTR_TELEPHONENUMBER"),
				 "homephone"			=> pql_get_define("PQL_ATTR_HOMEPHONE"),
				 "homepostaladdress"	=> pql_get_define("PQL_ATTR_HOMEPOSTALADDRESS"),
				 "secretary"			=> pql_get_define("PQL_ATTR_SECRETARY"),
				 "personaltitle"		=> pql_get_define("PQL_ATTR_PERSONALTITLE"),
				 "mobile"				=> pql_get_define("PQL_ATTR_MOBILE"),
				 "pager"				=> pql_get_define("PQL_ATTR_PAGER"),
				 "startwithadvancedmode"=> pql_get_define("PQL_ATTR_START_ADVANCED"));
foreach($attribs as $key => $attrib) {
	if($attrib == pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_GET["rootdn"]))
	  $got_user_reference_attribute = 1;

    $value = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], $attrib);
	if(is_array($value) and ($attrib != 'cn'))
	  // Only 'cn' is allowed to be multi-valued
	  $value = $value[0];
	elseif(!is_array($value) and ($attrib == 'cn'))
	  // He! 'cn' MUST be an array for the view to work!
	  $value = array($value);

	$$key = $value;
	$value = urlencode($value);

    // Setup edit links
    $link = $key . "_link";

	if($key == 'startwithadvancedmode')
	  // It's a toggle. Convert the boolean value to an integer
	  $$key = pql_format_bool($value);

	$alt = pql_complete_constant($LANG->_('Modify %attribute% for %what%'), array('attribute' => $attrib, 'what' => $username));
    $$link = "<a href=\"user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&attrib=$attrib&user=".$url["user"]."&$attrib=$value&view=" . $_GET["view"] . "\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt."\"></a>";
}
$quota = pql_user_get_quota($_pql->ldap_linkid, $_GET["user"]);

if(!$got_user_reference_attribute) {
	$key = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_GET["rootdn"]);
	$$key = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], $key);

	if(!$$key) {
		$$key = "<i>You're referencing users with the attribute <u>$key</u>, but it's not in the object</i>";
	}
}

// Some of these get set from the "$$key = $value[0]" line above.
if($userpassword == "") {
    $userpassword = $LANG->_('None');
} else {
    if(eregi("\{KERBEROS\}", $userpassword) or eregi("\{SASL\}", $userpassword)) {
		$princ = split("\}", $userpassword);
		$userpassword = $princ[1] . " (Kerberos V)";
    } else {
		$userpassword = $LANG->_('Encrypted');
    }
}

if($mailmessagestore == "") {
    $mailmessagestore = $LANG->_('None');
}

if($mailhost == "") {
    $mailhost = $LANG->_('None');
}

$controladmins = pql_get_attribute($_pql->ldap_linkid, $_GET["rootdn"], pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"));
if(is_array($controladmins)) {
	foreach($controladmins as $admin)
	  if($admin == $_GET["user"])
		$controlsadministrator = 1;
} elseif($controladmins == $_GET["user"]) {
	$controlsadministrator = 1;
}
// }}}

// {{{ Load groups user is member of
// If this user have a username (ie, 'uid') then let's see if this user is member
// of more groups (listed in the 'memberUid' attribute).
if($uid) {
	$memberuid = array();
	foreach($_SESSION["BASE_DN"] as $base) {
		$base  = urldecode($base);
// BUG: This won't work - wrong params!!
		$muids = pql_search($_pql->ldap_linkid, $base,
							pql_get_define("PQL_ATTR_ADDITIONAL_GROUP")."=".$uid,
							pql_get_define("PQL_ATTR_CN"));

		for($i=0; isset($muids[$i]); $i++)
		  $memberuid[] = $muids[$i];
	}
}
// }}}

// {{{ Get the object classes of this user
// If anyone of them is 'qmailGroup', then it's a Group object!
$objectclasses = pql_get_attribute($_pql->ldap_linkid, $_GET["user"],
								   pql_get_define("PQL_ATTR_OBJECTCLASS"));
if($objectclasses and !is_array($objectclasses)) {
  // Make it an array for the following foreach() to work..
  $objectclasses = array($objectclasses);
}
foreach($objectclasses as $oc) {
	if(eregi('qmailGroup', $oc))
	  $USER_IS_GROUP = 1;
}
// }}}

// {{{ Setup the buttons
$buttons = array('basic'			=> 'User data');
if(!$USER_IS_GROUP) {
	$new = array('personal'			=> 'Personal details',
				 'status'			=> 'Account status',
				 'delivery'			=> 'Delivery mode');
	$buttons = $buttons + $new;
} else {
	$new = array('group'			=> 'Group stuff');
	$buttons = $buttons + $new;
}

$new = array('email'				=> 'Registred addresses',
			 'forwards_from'		=> 'Forwarders from other accounts');
$buttons = $buttons + $new;

if(!$USER_IS_GROUP) {
	$new = array('forwards_to'		=> 'Forwarders to other accounts',
				 'antispam'			=> 'Antispam configuration');
	$buttons = $buttons + $new;
}

if($_SESSION["ADVANCED_MODE"] and !$USER_IS_GROUP) {
	$new = array('delivery_advanced'=> 'Advanced delivery properties',
				 'mailbox'			=> 'Mailbox properties',
				 'access'			=> 'User access');
	$buttons = $buttons + $new;

	if($_SESSION["ALLOW_BRANCH_CREATE"] && $_SESSION["ACI_SUPPORT_ENABLED"]) {
		$new = array('aci'			=> 'Access Control Information');
		$buttons = $buttons + $new;
	}
}

if(!$_SESSION["SINGLE_USER"]) {
	$new = array('actions'			=> 'Actions');
	$buttons = $buttons + $new;
}

pql_generate_button($buttons, "user=".$url["user"]); echo "  <br>\n";
// }}}

// {{{ Load the correct view page
if($_GET["view"] == 'basic')					include("./tables/user_details-basic.inc");
if($_GET["view"] == 'personal')					include("./tables/user_details-personal.inc");
if($_GET["view"] == 'email')					include("./tables/user_details-email.inc");
if($_GET["view"] == 'status')					include("./tables/user_details-status.inc");
if($_GET["view"] == 'delivery')					include("./tables/user_details-delivery.inc");
if($_GET["view"] == 'group')					include("./tables/user_details-group.inc");
if($_SESSION["ADVANCED_MODE"]) {
	if($_GET["view"] == 'delivery_advanced')	include("./tables/user_details-delivery_advanced.inc");
	if($_GET["view"] == 'mailbox')				include("./tables/user_details-mailbox.inc");
}
if($_GET["view"] == 'forwards_from')			include("./tables/user_details-forwards_from.inc");
if($_GET["view"] == 'forwards_to')				include("./tables/user_details-forwards_to.inc");
if($_GET["view"] == 'antispam')					include("./tables/user_details-antispam.inc");
if($_SESSION["ADVANCED_MODE"] and !$_SESSION["SINGLE_USER"]) {
	if($_GET["view"] == 'access')				include("./tables/user_details-access.inc");
	if($_GET["view"] == 'aci')					include("./tables/user_details-aci.inc");
}
if(!$_SESSION["SINGLE_USER"]) {
	if($_GET["view"] == 'actions')				include("./tables/user_details-action.inc");
}
// }}}

if(file_exists($_SESSION["path"]."/.DEBUG_ME") and file_exists($_SESSION["path"]."/.DEBUG_PROFILING")) {
  $now = pql_format_return_unixtime(); echo "Now: <b>$now</b><br>";
}
?>
</body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
