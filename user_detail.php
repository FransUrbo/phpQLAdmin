<?php
// shows details of a user
// $Id: user_detail.php,v 2.70 2004-03-11 18:13:32 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

if(!$_GET["rootdn"]) {
	$_GET["rootdn"] = pql_get_rootdn($_GET["user"], 'user_detail.php');
}

$url["domain"] = pql_format_urls($_GET["domain"]);
$url["user"]   = pql_format_urls($_GET["user"]);
$url["rootdn"] = pql_format_urls($_GET["rootdn"]);

// Get default domain name for this domain
if($_GET["domain"]) {
	$defaultdomain = pql_domain_get_value($_pql, $_GET["domain"], pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
}

include("./header.html");

// print status message, if one is available
if(isset($_GET["msg"])) {
	pql_format_status_msg($_GET["msg"]);
}

// reload navigation bar if needed
if(isset($rlnb) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
	if($rlnb == 1) {
?>
  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    refreshFrames();
  //--></script>
<?php
	} elseif($rlnb == 2) {
?>
  <script language="JavaScript1.2"><!--
    // reload navigation frame
    parent.frames.pqlnav.location.reload();
  //--></script>
<?php   }
}

$username = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_CN"));
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_UID"));
}
$username = $username[0];
?>

  <span class="title1"><?=$username?></span>
  <br><br>
<?php
// check if user exists
if(!pql_user_exist($_pql->ldap_linkid, $_GET["rootdn"], $_GET["user"])) {
    echo pql_complete_constant($LANG->_('User %user% does not exist'), array('user' => '<u>'.$_GET["user"].'</u>'));
    exit();
}


/* DLW: Setting all of these variables in the main code is pointless.  Most of them
 *      never get used.  They should be handled by the tables
 *      that actually request the variables (user_details-mailbox.inc). */

// Get basic user information
// Some of these (everything after the 'homedirectory')
// uses 'objectClass: pilotPerson' -> http://rfc-1274.rfcindex.net/
$attribs = array(pql_get_define("PQL_ATTR_CN"),
				 pql_get_define("PQL_ATTR_SN"),
				 pql_get_define("PQL_ATTR_QMAILUID"),
				 pql_get_define("PQL_ATTR_QMAILGID"),
				 pql_get_define("PQL_ATTR_LOGINSHELL"),
				 pql_get_define("PQL_ATTR_UID"),
				 pql_get_define("PQL_ATTR_PASSWD"),
				 pql_get_define("PQL_ATTR_MAILSTORE"),
				 pql_get_define("PQL_ATTR_MAILHOST"),
				 pql_get_define("PQL_ATTR_HOMEDIR"),
				 pql_get_define("PQL_ATTR_ROOMNUMBER"),
				 pql_get_define("PQL_ATTR_TELEPHONENUMBER"),
				 pql_get_define("PQL_ATTR_HOMEPHONE"),
				 pql_get_define("PQL_ATTR_HOMEPOSTALADDRESS"),
				 pql_get_define("PQL_ATTR_SECRETARY"),
				 pql_get_define("PQL_ATTR_PERSONALTITLE"),
				 pql_get_define("PQL_ATTR_MOBILE"),
				 pql_get_define("PQL_ATTR_PAGER"));
foreach($attribs as $attrib) {
    $attrib = lc($attrib);

    $value = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], $attrib);
    $$attrib = $value[0];
    $value = urlencode($$attrib);

    // Setup edit links
    $link = $attrib . "_link";

	$alt = pql_complete_constant($LANG->_('Modify %attribute% for %what%'), array('attribute' => $attrib, 'what' => $username));
    $$link = "<a href=\"user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&attrib=$attrib&user=".$url["user"]."&$attrib=$value&view=$view\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt."\"></a>";
}
$quota = pql_user_get_quota($_pql->ldap_linkid, $_GET["user"]);

// Some of these get set from the "$$attrib = $value[0]" line above.
if($userpassword == "") {
    $userpassword = $LANG->_('None');
} else {
    if(eregi("\{KERBEROS\}", $userpassword)) {
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

$controladmins = pql_domain_get_value($_pql, $_GET["rootdn"], pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"));
if(is_array($controladmins)) {
	foreach($controladmins as $admin)
	  if($admin == $_GET["user"])
		$controlsadministrator = 1;
} elseif($controladmins == $_GET["user"]) {
	$controlsadministrator = 1;
}

// If this user have a username (ie, 'uid') then let's see if this user is member
// of more groups (listed in the 'memberUid' attribute).
if($uid) {
	$memberuid = array();
	foreach($_pql->ldap_basedn as $base) {
		$base  = urldecode($base);
		$muids = pql_search($_pql->ldap_linkid, $base,
							pql_get_define("PQL_ATTR_ADDITIONAL_GROUP")."=".$uid,
							pql_get_define("PQL_ATTR_CN"));

		for($i=0; $muids[$i]; $i++)
		  $memberuid[] = $muids[$i];
	}
}

// Setup the buttons
$buttons = array('basic'			=> 'User data',
				 'personal'			=> 'Personal details',
				 'email'			=> 'Registred addresses',
				 'status'			=> 'Account status',
				 'delivery'			=> 'Delivery mode',
				 'forwards_from'	=> 'Forwarders from other accounts',
				 'forwards_to'		=> 'Forwarders to other accounts');

if($_SESSION["ADVANCED_MODE"]) {
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

if($_GET["view"] == '')
	$_GET["view"] = 'basic';

if($_GET["view"] == 'basic')					include("./tables/user_details-basic.inc");
if($_GET["view"] == 'personal')					include("./tables/user_details-personal.inc");
if($_GET["view"] == 'email')					include("./tables/user_details-email.inc");
if($_GET["view"] == 'status')					include("./tables/user_details-status.inc");
if($_GET["view"] == 'delivery')					include("./tables/user_details-delivery.inc");
if($_SESSION["ADVANCED_MODE"]) {
	if($_GET["view"] == 'delivery_advanced')	include("./tables/user_details-delivery_advanced.inc");
	if($_GET["view"] == 'mailbox')				include("./tables/user_details-mailbox.inc");
}
if($_GET["view"] == 'forwards_from')			include("./tables/user_details-forwards_from.inc");
if($_GET["view"] == 'forwards_to')				include("./tables/user_details-forwards_to.inc");
if($_SESSION["ADVANCED_MODE"] and !$_SESSION["SINGLE_USER"]) {
	if($_GET["view"] == 'access')				include("./tables/user_details-access.inc");
	if($_GET["view"] == 'aci')					include("./tables/user_details-aci.inc");
}
if(!$_SESSION["SINGLE_USER"]) {
	if($_GET["view"] == 'actions')				include("./tables/user_details-action.inc");
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
</body>
</html>
