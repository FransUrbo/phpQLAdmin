<?php
// shows details of a user
// user_detail.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

if(!$rootdn) {
	$rootdn = pql_get_rootdn($user);
}

// Get default domain name for this domain
$defaultdomain = pql_get_domain_value($_pql, $domain, "defaultdomain");

include("./header.html");

// print status message, if one is available
if(isset($msg)) {
	print_status_msg($msg);
}

// reload navigation bar if needed
if(isset($rlnb) and $config["PQL_GLOB_AUTO_RELOAD"]) {
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

$username = pql_get_userattribute($_pql->ldap_linkid, $user, 'cn');
if(!$username[0]) {
    // No common name, use uid field
    $username = pql_get_userattribute($_pql->ldap_linkid, $user, 'uid');
}
$username = maybe_decode($username[0]);
?>

  <span class="title1"><?=$username?></span>
  <br><br>
<?php
// check if user exists
if(!pql_user_exist($_pql->ldap_linkid, $user)) {
    echo "User &quot;$user&quot; does not exist";
    exit();
}

// Get basic user information
// Some of these (everything after the 'homedirectory')
// uses 'objectClass: pilotPerson' -> http://rfc-1274.rfcindex.net/
$attribs = array('cn', 'sn', 'uidNumber', 'gidNumber', 'loginShell', 'uid',
				 'userPassword', 'mailMessageStore', 'mailHost', 'homeDirectory',
				 'roomNumber', 'telePhoneNumber', 'homePhone', 'homePostalAddress',
				 'secretary', 'personalTitle', 'mobile', 'pager');
foreach($attribs as $attrib) {
    $attrib = strtolower($attrib);

    $value = pql_get_userattribute($_pql->ldap_linkid, $user, $attrib);
    $$attrib = maybe_decode($value[0]);
    $value = urlencode($$attrib);

    // Setup edit links
    $link = $attrib . "_link";
    $$link = "<a href=\"user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=$domain&attrib=$attrib&user=<?php echo urlencode($user); ?>&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify $attrib for $username\"></a>";
}
$quota = pql_get_userquota($_pql->ldap_linkid, $user);

if($userpassword == "") {
    $userpassword = PQL_LANG_USERPASSWORD_NONE;
} else {
    if(eregi("\{KERBEROS\}", $userpassword)) {
		$princ = split("\}", $userpassword);
		$userpassword = $princ[1] . " " . PQL_LANG_USERPASSWORD_KERBEROS;
    } else {
		$userpassword = PQL_LANG_USERPASSWORD_ENCRYPTED;
    }
}

if($mailmessagestore == "") {
    $mailmessagestore = PQL_LANG_MAILMESSAGESTORE_NONE;
}

if($mailhost == "") {
    $mailhost = PQL_LANG_MAILHOST_NONE;
}

$mailmessagestore = maybe_decode($mailmessagestore);
$mailhost = maybe_decode($mailhost);

$userdn = urlencode($user);
?>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%" colspan="2"><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=basic"?>"><img alt="/ User Data \" vspace="0" hspace="0" border="0" src="navbutton.php?User Data"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=personal"?>"><img alt="/ Personal Details \" vspace="0" hspace="0" border="0" src="navbutton.php?Personal Details"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=email"?>"><img alt="/ Registred Addresses \" vspace="0" hspace="0" border="0" src="navbutton.php?Registred Addresses"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=status"?>"><img alt="/ Account Status \" vspace="0" hspace="0" border="0" src="navbutton.php?Account Status"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=delivery"?>"><img alt="/ Delivery Mode \" vspace="0" hspace="0" border="0" src="navbutton.php?Delivery Mode"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=delivery_advanced"?>"><img alt="/ Advanced Delivery Properties \" vspace="0" hspace="0" border="0" src="navbutton.php?Delivery Properties"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=mailbox"?>"><img alt="/ Mailbox properties \" vspace="0" hspace="0" border="0" src="navbutton.php?Mailbox properties"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=forwards_from"?>"><img alt="/ Forwarders from other accounts \" vspace="0" hspace="0" border="0" src="navbutton.php?Forwards from account(s)"></a><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=forwards_to"?>"><img alt="/ Forwarding to other account \" vspace="0" hspace="0" border="0" src="navbutton.php?Forwards to account(s)"></a><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=access"?>"><img alt="/ User Access \" vspace="0" hspace="0" border="0" src="navbutton.php?User Access"></a><?php if(!$SINGLE_USER) { ?><br><a href="<?=$PHP_SELF."?rootdn=$rootdn&domain=$domain&user=$userdn&view=actions"?>"><img alt="/ Actions \" vspace="0" hspace="0" border="0" src="navbutton.php?Actions"></a><?php } ?></td>
  </tr>
</table>

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
