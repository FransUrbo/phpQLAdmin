<?php
// attribute plugin for
// deliverymode (delivery preferences)
// $Id: attrib.deliverymode.inc,v 2.14 2003-11-18 13:16:38 turbo Exp $

function attribute_check($type) {
	global $error, $reply, $mailreplytext, $LANG;

	// if reply is selected, mailreplytext must not be null
	if($reply != "" and $mailreplytext == "") {
		$error["mailreplytext"] = $LANG->_('Missing');
		return false;
	}

	return true;
}

function attribute_init() {
	global $reply, $mailreplytext, $deliverymode, $domain;
	global $user, $_pql;

	// initialize values directly from ldap
	$mode = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MODE"));

	// delivery profile
	if(is_array($mode) and in_array("forwardonly",$mode))
	  $deliverymode = "forward";
	else
	  $deliverymode = "local";
	
	// reply / mailreplytext
	if(is_array($mode) and in_array("reply", $mode))
	  $reply = "ok";
	
	$mailreplytext = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_REPLYTEXT"));
	$mailreplytext = $mailreplytext[0];
}

function attribute_print_form($type = "modify") {
	global $reply, $mailreplytext, $deliverymode, $domain, $user, $PHP_SELF, $attrib, $error, $rootdn, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Delivery mode')?></th>
<?php if(!$GLOBALS["SINGLE_USER"]) { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Delivery mode')?></td>
          <td>
            <?php echo format_error($error["deliverymode"]); ?>
            <select name="deliverymode">
              <option value="local" <?php if($deliverymode == "local"){ echo "SELECTED";} ?>><?=$LANG->_('Mail account')?>
              <option value="forward" <?php if($deliverymode == "forward"){ echo "SELECTED";} ?>><?=$LANG->_('Forward account')?>
            </select>
          </td>
        </tr>

<?php } ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Do auto reply')?></td>
          <td><input type="checkbox" name="reply" value="ok" <?php if($reply=="ok"){ echo "checked";} ?>><?=$LANG->_('Send an auto reply')?></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Text for replying')?></td>
          <td valign="center"><?php echo format_error($error["mailreplytext"]); ?> <textarea name="mailreplytext" cols="40" rows="10"><?=$mailreplytext?></textarea></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <table>
            <td><img src="images/info.png" width="16" height="16" border="0" align="left"></td>
            <td>
              <?=$LANG->_('If send auto reply is enabled, QmailLDAP will first store the mail in the users mailbox, THEN send the reply')?>.
            </td>
          </table>
        </tr>
      </th>
    </table>

<?php if($GLOBALS["SINGLE_USER"]) { ?>
    <input type="hidden" name="deliverymode" value="<?=$deliverymode?>">
<?php } ?>
    <input type="hidden" name="submit"       value="1">
    <input type="hidden" name="attrib"       value="<?=$attrib?>">
    <input type="hidden" name="oldvalue"     value="<?=$oldvalue?>">
    <input type="hidden" name="domain"       value="<?=$domain?>">
    <input type="hidden" name="rootdn"       value="<?=$rootdn?>">
    <input type="hidden" name="user"         value="<?=$user?>">
    <input type="hidden" name="view"         value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}

function attribute_save($type) {
	global $domain, $user, $attrib, $reply, $mailreplytext, $deliverymode, $_pql, $view, $LANG;
	
	switch($deliverymode) {
	  case "forward":
		$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "forwardonly";
		$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "nombox";
		break;

	  case "local":
		$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "localdelivery";
	}
	
	if($reply == "ok")
	  $entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "reply";

	if (!empty($mailreplytext)) 
	  $entry[pql_get_define("PQL_GLOB_ATTR_REPLYTEXT")] = preg_replace('/\\\/', "", $mailreplytext);
	else {
		// Check if the entry in database is empty. 
		// If not, we have choosen to DELETE the value
		$mrt_db = pql_get_userattribute($_pql->ldap_linkid,
										$user, pql_get_define("PQL_GLOB_ATTR_REPLYTEXT"));
		if($mrt_db[0])
		  pql_replace_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_REPLYTEXT"));
	}
	
	switch($type) {
	  case "fulldomain":
		$users = pql_get_user($_pql->ldap_linkid, $domain);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_attribute($_pql->ldap_linkid, $user, '', $entry);
			
			if(in_array(false, $return))
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('delivery mode'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
			else
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('delivery mode')));
			
			attribute_forward($msg);
		}
		break;
		
	  case "modify":
		if(pql_replace_attribute($_pql->ldap_linkid, $user, '', $entry)) 
		  $msg = pql_complete_constant($LANG->_('successfully changed %what%'),
									   array('what' => $LANG->_('delivery mode')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('delivery mode'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
