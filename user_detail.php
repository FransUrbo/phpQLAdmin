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
$username = $username[0];
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
    $$attrib = utf8_decode($value[0]);
    $value = urlencode($$attrib);

    // Setup edit links
    $link = $attrib . "_link";
    $$link = "<a href=\"user_edit_attribute.php?rootdn=<?=$rootdn?>&domain=$domain&attrib=$attrib&user=$user&$attrib=$value\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"Modify $attrib for $username\"></a>";
}
$quota = pql_get_userquota($_pql->ldap_linkid, $user);

if($userpassword == "") {
    $userpassword = PQL_LANG_USERPASSWORD_NONE;
} else {
    if(eregi("{KERBEROS}", $userpassword)) {
		$princ = split("}", $userpassword);
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
?>

<?php include("./tables/user_details-basic.inc"); ?>
<?php include("./tables/user_details-personal.inc"); ?>
<?php include("./tables/user_details-email.inc"); ?>
<?php include("./tables/user_details-forwards_from.inc"); ?>
<?php include("./tables/user_details-status.inc"); ?>
<?php if($ADVANCED_MODE) { ?>
<?php	include("./tables/user_details-delivery.inc"); ?>
<?php	include("./tables/user_details-delivery_advanced.inc"); ?>
<?php	include("./tables/user_details-mailbox.inc"); ?>
<?php } ?>
<?php include("./tables/user_details-forwards_to.inc"); ?>
<?php if($ADVANCED_MODE and !$SINGLE_USER) { ?>
<?php	include("./tables/user_details-access.inc"); ?>
<?php } ?>

<?php if(!$SINGLE_USER) { ?>
  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_LANG_ACTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="user_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>&user=<?=$user?>"><?=PQL_LANG_USER_DELETE?></a></td>
      </tr>
    </th>
  </table>
<?php }

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
