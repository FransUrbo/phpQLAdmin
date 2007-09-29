<?php
// Get toggles and single valued attributes
$attribs = array("senderconfirm"	=> pql_get_define("PQL_ATTR_GROUP_CONFIRM"),
				 "membersonly"		=> pql_get_define("PQL_ATTR_GROUP_MEMBERS_ONLY"),
				 "confirmtext"		=> pql_get_define("PQL_ATTR_GROUP_CONFIRM_TEXT"),
				 "moderatortext"	=> pql_get_define("PQL_ATTR_GROUP_MODERATOR_TEXT"));
foreach($attribs as $key => $attrib) {
	// Get default value
	$value = $_pql->get_attribute($_REQUEST["user"], $attrib);
	if(($key == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_CONFIRM"))) or ($key == vsprintf("%s", pql_get_define("PQL_ATTR_GROUP_MEMBERS_ONLY")))) {
	    // It's a user toggle
		$$key = $value[0];
	    if(!$$key)
	      $$key = 0;
	    else
	      // Convert the boolean value to an integer
	      $$key = pql_format_bool($$key);
	} elseif(($key == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_CONFIRM_TEXT"))) or ($key == vsprintf("%s", pql_get_define("PQL_ATTR_GROUP_MODERATOR_TEXT"))))
	  // It's a multi-line text
	  $$key = eregi_replace("\n", "<br>", $value);

	// Generate the link URL 
	$link   = $key . "_link";
	$$link  = "<a href=\"user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&attrib=$attrib";
	$$link .= "&user=".$url["user"]."&oldvalue=".$$attrib."&view=".$_GET["view"]."\"><img src=\"images/edit.png\" ";
	$$link .= "width=\"12\" height=\"12\" alt=\"".$LANG->_('Change value')."\" border=\"0\"></a>";
	if(($key == sprintf("%s", pql_get_define("PQL_ATTR_GROUP_CONFIRM_TEXT"))) or ($key == vsprintf("%s", pql_get_define("PQL_ATTR_GROUP_MODERATOR_TEXT")))) {
		$$link .= "&nbsp;<a href=\"user_del_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&attrib=$attrib";
		$$link .= "&user=".$url["user"]."&oldvalue=".$$attrib."&view=".$_GET["view"]."&ok=1\"><img src=\"images/del.png\" ";
		$$link .= "width=\"12\" height=\"12\" alt=\"".$LANG->_('Delete value')."\" border=\"0\"></a>";
	}
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
	if($value = $_pql->get_attribute($_REQUEST["user"], $attrib)) {
	  if(!is_array($value))
		$$key  = array($value);
	  else
		$$key  = $value;
	} else
	  $$key	   = '';
}

$url_tmp    = "user_edit_attribute.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&user=".$url["user"]."&";
?>
  <!-- Qmail-Group stuff (kind'a like a mailinglist, only more annoying) ' -->
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
    <th colspan="3" align="left"><?=$LANG->_('Subscribed users/addresses')."\n"?>
<?php
// DNMember
$txt["tit"] = $LANG->_('Member DN(s)');
$txt["add"] = $LANG->_('Add member');
$txt["del"] = $LANG->_('Delete member');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=dnmember";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=dnmember";
pql_format_table_single($txt, $url, $dnmember, 'dnmember');

// FilterMember
$txt["tit"] = $LANG->_('Filter DN(s)');
$txt["add"] = $LANG->_('Add filter');
$txt["del"] = $LANG->_('Delete filter');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=filtermember";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=filtermember";
pql_format_table_single($txt, $url, $filtermember, 'filtermember');

// RFC822Member
$txt["tit"] = $LANG->_('Subscriber addresses');
$txt["add"] = $LANG->_('Add subscriber');
$txt["del"] = $LANG->_('Delete subscriber');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=rfc822member";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=rfc822member";
pql_format_table_single($txt, $url, $rfc822member, 'rfc822member');
?>
    </th>

    <th><tr></tr></th>
    <th colspan="3" align="left"><?=$LANG->_('Senders allowed to send to group')."\n"?>
<?php
// DNSender
$txt["tit"] = $LANG->_('Sender DN(s)');
$txt["add"] = $LANG->_('Add sender');
$txt["del"] = $LANG->_('Delete sender');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=dnsender";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=dnsender";
pql_format_table_single($txt, $url, $dnsender, 'dnsender');

// FilterSender
$txt["tit"] = $LANG->_('Filter DN(s)');
$txt["add"] = $LANG->_('Add filter');
$txt["del"] = $LANG->_('Delete filter');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=filtersender";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=filtersender";
pql_format_table_single($txt, $url, $filtersender, 'filtersender');

// RFC822Sender
$txt["tit"] = $LANG->_('Allowed sender address(es)');
$txt["add"] = $LANG->_('Add allowed sender');
$txt["del"] = $LANG->_('Delete allowed sender');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=rfc822sender";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=rfc822sender";
pql_format_table_single($txt, $url, $rfc822sender, 'rfc822sender');
?>
    </th>

    <th><tr></tr></th>
    <th colspan="3" align="left"><?=$LANG->_('Group moderators')."\n"?>
<?php
// DNModerator
$txt["tit"] = $LANG->_('Moderator DN(s)');
$txt["add"] = $LANG->_('Add moderator');
$txt["del"] = $LANG->_('Delete moderator');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=dnmoderator";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=dnmoderator";
pql_format_table_single($txt, $url, $dnmoderator, 'dnmoderator');

// RFC822Moderator
$txt["tit"] = $LANG->_('Moderator address(es)');
$txt["add"] = $LANG->_('Add moderator');
$txt["del"] = $LANG->_('Delete moderator');
$url["add"] = $url_tmp . "submit=3&action=add&view=".$_REQUEST["view"]."&attrib=rfc822moderator";
$url["del"] = $url_tmp . "submit=4&action=delete&view=".$_REQUEST["view"]."&attrib=rfc822moderator";
pql_format_table_single($txt, $url, $rfc822moderator, 'rfc822moderator');
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
