<?php
// shows details of a user
// user_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// Make sure we can have a ' in branch (also affects the user DN).
$user   = eregi_replace("\\\'", "'", $user);
$domain = eregi_replace("\\\'", "'", $domain);

if(!$rootdn) {
	$rootdn = pql_get_rootdn($user);
}

// Get default domain name for this domain
if($domain) {
	$defaultdomain = pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"));
}

include("./header.html");

// print status message, if one is available
if(isset($msg)) {
	print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and pql_get_define("PQL_GLOB_AUTO_RELOAD")) {
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

$username = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_CN"));
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_UID"));
}
$username = $username[0];
?>

  <span class="title1"><?=$username?></span>
  <br><br>
<?php
// check if user exists
if(!pql_user_exist($_pql->ldap_linkid, $user)) {
    echo pql_complete_constant($LANG->_('User %user% does not exist'), array('user' => '<u>'.$user.'</u>'));
    exit();
}

// Get basic user information
// Some of these (everything after the 'homedirectory')
// uses 'objectClass: pilotPerson' -> http://rfc-1274.rfcindex.net/
$attribs = array(pql_get_define("PQL_GLOB_ATTR_CN"),
				 pql_get_define("PQL_GLOB_ATTR_SN"),
				 pql_get_define("PQL_GLOB_ATTR_QMAILUID"),
				 pql_get_define("PQL_GLOB_ATTR_QMAILGID"),
				 pql_get_define("PQL_GLOB_ATTR_LOGINSHELL"),
				 pql_get_define("PQL_GLOB_ATTR_UID"),
				 pql_get_define("PQL_GLOB_ATTR_PASSWD"),
				 pql_get_define("PQL_GLOB_ATTR_MAILSTORE"),
				 pql_get_define("PQL_GLOB_ATTR_MAILHOST"),
				 pql_get_define("PQL_GLOB_ATTR_HOMEDIR"),
				 pql_get_define("PQL_GLOB_ATTR_ROOMNUMBER"),
				 pql_get_define("PQL_GLOB_ATTR_TELEPHONENUMBER"),
				 pql_get_define("PQL_GLOB_ATTR_HOMEPHONE"),
				 pql_get_define("PQL_GLOB_ATTR_HOMEPOSTALADDRESS"),
				 pql_get_define("PQL_GLOB_ATTR_SECRETARY"),
				 pql_get_define("PQL_GLOB_ATTR_PERSONALTITLE"),
				 pql_get_define("PQL_GLOB_ATTR_MOBILE"),
				 pql_get_define("PQL_GLOB_ATTR_PAGER"));
foreach($attribs as $attrib) {
    $attrib = strtolower($attrib);

    $value = pql_get_userattribute($_pql->ldap_linkid, $user, $attrib);
    $$attrib = $value[0];
    $value = urlencode($$attrib);

    // Setup edit links
    $link = $attrib . "_link";
	$urluser = urlencode($user);

	$alt = pql_complete_constant($LANG->_('Modify %attribute% for %what%'), array('attribute' => $attrib, 'what' => $username));
    $$link = "<a href=\"user_edit_attribute.php?rootdn=$rootdn&domain=$domain&attrib=$attrib&user=$urluser&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"".$alt."\"></a>";
}
$quota = pql_get_userquota($_pql->ldap_linkid, $user);

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

$userdn = urlencode($user);

$controladmins = pql_get_domain_value($_pql, $rootdn, pql_get_define("PQL_GLOB_ATTR_CONTROLSADMINISTRATOR"));
if(is_array($controladmins)) {
	foreach($controladmins as $admin)
	  if($admin == $user)
		$controlsadministrator = 1;
} elseif($controladmins == $user) {
	$controlsadministrator = 1;
}
?>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%" colspan="2"><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=basic"?>"><img alt="/ <?=$LANG->_('User data')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('User data')?>"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=personal"?>"><img alt="/ <?=$LANG->_('Personal details')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Personal details')?>"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=email"?>"><img alt="/ <?=$LANG->_('Registred addresses')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Registred addresses')?>"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=status"?>"><img alt="/ <?=$LANG->_('Account status')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Account status')?>"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=delivery"?>"><img alt="/ <?=$LANG->_('Delivery mode')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Delivery mode')?>"></a><?php if($ADVANCED_MODE) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=delivery_advanced"?>"><img alt="/ <?=$LANG->_('Advanced delivery properties')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Advanced delivery properties')?>"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=mailbox"?>"><img alt="/ <?=$LANG->_('Mailbox properties')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Mailbox properties')?>"></a><?php } ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=forwards_from"?>"><img alt="/ <?=$LANG->_('Forwarders from other accounts')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Forwarders from other accounts')?>"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=forwards_to"?>"><img alt="/ <?=$LANG->_('Forwarders to other accounts')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Forwarders to other accounts')?>"></a><?php if($ADVANCED_MODE) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=access"?>"><img alt="/ <?=$LANG->_('User access')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('User access')?>"></a><br><?php if($ALLOW_BRANCH_CREATE) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=aci"?>"><img alt="/ <?=$LANG->_('Access Control Information')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Access Control Information')?>"></a><?php } ?><?php } ?><?php if(!$SINGLE_USER) { ?><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=actions"?>"><img alt="/ <?=$LANG->_('Actions')?> \" vspace="0" hspace="0" border="0" src="navbutton.php?<?=$LANG->_('Actions')?>"></a><?php } ?></td>
  </tr>
</table>

<br>
<?php
if($view == '')
	$view = 'basic';

if($view == 'basic')					include("./tables/user_details-basic.inc");
if($view == 'personal')					include("./tables/user_details-personal.inc");
if($view == 'email')					include("./tables/user_details-email.inc");
if($view == 'status')					include("./tables/user_details-status.inc");
if($view == 'delivery')					include("./tables/user_details-delivery.inc");
if($ADVANCED_MODE) {
	if($view == 'delivery_advanced')	include("./tables/user_details-delivery_advanced.inc");
	if($view == 'mailbox')				include("./tables/user_details-mailbox.inc");
}
if($view == 'forwards_from')			include("./tables/user_details-forwards_from.inc");
if($view == 'forwards_to')				include("./tables/user_details-forwards_to.inc");
if($ADVANCED_MODE and !$SINGLE_USER) {
	if($view == 'access')				include("./tables/user_details-access.inc");
	if($view == 'aci')					include("./tables/user_details-aci.inc");
}
if(!$SINGLE_USER) {
	if($view == 'actions')				include("./tables/user_details-action.inc");
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
