<?php
// Get toggles and single valued attributes
$attribs = array("senderconfirm"	=> pql_get_define("PQL_ATTR_GROUP_CONFIRM"),
				 "membersonly"		=> pql_get_define("PQL_ATTR_GROUP_MEMBERS_ONLY"),
				 "confirmtext"		=> pql_get_define("PQL_ATTR_GROUP_CONFIRM_TEXT"),
				 "moderatortext"	=> pql_get_define("PQL_ATTR_GROUP_MODERATOR_TEXT"));
foreach($attribs as $key => $attrib) {
	// Get default value
	$value = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], $attrib);
	if(($key == 'senderconfirm') or ($key == 'membersonly')) {
	    // It's a user toggle
		$$key = $value[0];
	    if(!$$key)
	      $$key = 0;
	    else
	      // Convert the boolean value to an integer
	      $$key = pql_format_bool($$key);
	} elseif(($key == 'confirmtext') or ($key == 'moderatortext'))
	  // It's a multi-line text
	  $$key = eregi_replace("\n", "<br>", $value[0]);

	// Generate the link URL 
	$link   = $key . "_link";
	$$link  = "<a href=\"user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&attrib=$attrib";
	$$link .= "&user=".$url["user"]."&oldvalue=".$$attrib."&view=".$_GET["view"]."\"><img src=\"images/edit.png\" ";
	$$link .= "width=\"12\" height=\"12\" alt=\"".$LANG->_('Change value')."\" border=\"0\"></a>";
}

// Get multi valued attributes
$attribs = array("dnmember"			=> pql_get_define("PQL_ATTR_GROUP_DN_MEMBER"),
				 "dnsender"			=> pql_get_define("PQL_ATTR_GROUP_DN_SENDER"),
				 "dnmoderator"		=> pql_get_define("PQL_ATTR_GROUP_DN_MODERATOR"),
				 "filtermember"		=> pql_get_define("PQL_ATTR_GROUP_FILTER_MEMBER"),
				 "filtersender"		=> pql_get_define("PQL_ATTR_GROUP_FILTER_SENDER"),
				 "rfc822member"		=> pql_get_define("PQL_ATTR_GROUP_822_MEMBER"),
				 "rfc822sender"		=> pql_get_define("PQL_ATTR_GROUP_822_SENDER"),
				 "rfc822moderator"	=> pql_get_define("PQL_ATTR_GROUP_822_MODERATOR"));
foreach($attribs as $key => $attrib) {
	// Get default value
	$value = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], $attrib);
	$$key  = $value;
}

$url_tmp    = "user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&user=".$url["user"]."&";
?>
  <!-- Qmail-Group stuff (kind'a like a mailinglist, only more annoying) -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Group data')?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Confirm sender')?></td>
        <td><?php if($senderconfirm){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?=$senderconfirm_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Member only posts')?></td>
        <td><?php if($membersonly){echo $LANG->_('Yes');}else{echo $LANG->_('No');} ?></td>
        <td><?=$membersonly_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Sender confirmation text')?></td>
        <td><?php if(!$confirmtext) { echo "<i>not set</i>"; } else { echo $confirmtext; }?></td>
        <td><?=$confirmtext_link?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Moderator confirmation text')?></td>
        <td><?php if(!$moderatortext) { echo "<i>not set</i>"; } else { echo $moderatortext; }?></td>
        <td><?=$moderatortext_link?></td>
      </tr>
    </th>
  </table>

  <P>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Group members')."\n"?>
<?php
// DNMember
$txt["tit"] = $LANG->_('Member DN(s)');
$txt["add"] = $LANG->_('Add member');
$txt["del"] = $LANG->_('Delete member');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=dnmember";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=dnmember";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $dnmember, 'dnmember');

// FilterMember
$txt["tit"] = $LANG->_('Filter DN(s)');
$txt["add"] = $LANG->_('Add filter');
$txt["del"] = $LANG->_('Delete filter');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=filtermember";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=filtermember";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $filtermember, 'filtermember');

// RFC822Member
$txt["tit"] = $LANG->_('RFC822 member DN(s)');
$txt["add"] = $LANG->_('Add rfc822 member');
$txt["del"] = $LANG->_('Delete rfc822 member');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=rfc822member";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=rfc822member";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $rfc822member, 'rfc822member');
?>
    </th>

    <th><tr></tr></th>
    <th colspan="3" align="left"><?=$LANG->_('Group senders')."\n"?>
<?php
// DNSender
$txt["tit"] = $LANG->_('Sender DN(s)');
$txt["add"] = $LANG->_('Add sender');
$txt["del"] = $LANG->_('Delete sender');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=dnsender";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=dnsender";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $dnsender, 'dnsender');

// FilterSender
$txt["tit"] = $LANG->_('Filter DN(s)');
$txt["add"] = $LANG->_('Add filter');
$txt["del"] = $LANG->_('Delete filter');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=filtersender";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=filtersender";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $filtersender, 'filtersender');

// RFC822Sender
$txt["tit"] = $LANG->_('RFC822 sender DN(s)');
$txt["add"] = $LANG->_('Add rfc822 sender');
$txt["del"] = $LANG->_('Delete rfc822 sender');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=rfc822sender";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=rfc822sender";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $rfc822sender, 'rfc822sender');
?>
    </th>

    <th><tr></tr></th>
    <th colspan="3" align="left"><?=$LANG->_('Moderators')."\n"?>
<?php
// DNModerator
$txt["tit"] = $LANG->_('Moderator DN(s)');
$txt["add"] = $LANG->_('Add moderator');
$txt["del"] = $LANG->_('Delete moderator');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=dnmoderator";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=dnmoderator";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $dnmoderator, 'dnmoderator');

// RFC822Moderator
$txt["tit"] = $LANG->_('RFC822 Moderator DN(s)');
$txt["add"] = $LANG->_('Add RFC822 moderator');
$txt["del"] = $LANG->_('Delete RFC822 moderator');
$url["add"] = $url_tmp . "submit=3&action=add&view=$view&attrib=rfc822moderator";
$url["del"] = $url_tmp . "submit=4&action=delete&view=$view&attrib=rfc822moderator";
pql_format_table_single($_pql->ldap_linkid, $txt, $url, $rfc822moderator, 'rfc822moderator');
?>
    </th>
  </table>

  <br><br>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
