<?php
// attribute plugin for
// deliverymode (delivery preferences)
// $Id: attrib.deliverymode.inc,v 2.17.2.2 2003-12-09 21:10:29 dlw Exp $

function attribute_check($type) {
    global $error, $LANG;

	// if reply is selected, mailreplytext must not be null
	if(!empty($_POST["reply"]) and empty($_POST["mailreplytext"])) {
		$error["mailreplytext"] = $LANG->_('Missing');
		return false;
	}

	return true;
}

function attribute_init() {
    global $_pql;

	// initialize values directly from ldap
	$mode = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_GLOB_ATTR_MODE"));

	// delivery profile
	if(is_array($mode) and in_array("forwardonly",$mode))
	  $_REQUEST["deliverymode"] = "forward";
	else
	  $_REQUEST["deliverymode"] = "local";

	// reply / mailreplytext
	if(is_array($mode) and in_array("reply", $mode))
	  $_REQUEST["reply"] = "ok";
	
	$mailreplytext = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_GLOB_ATTR_REPLYTEXT"));
	$mailreplytext = $mailreplytext[0];
	$_REQUEST["mailreplytext"] = $mailreplytext;
}

function attribute_print_form($type = "modify") {
    global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Delivery mode')?></th>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Delivery mode')?></td>
          <td>
            <?php echo pql_format_error_span($error["deliverymode"]); ?>
            <select name="deliverymode">
              <option value="local" <?php if($_REQUEST["deliverymode"] == "local"){ echo "SELECTED";} ?>><?=$LANG->_('Mail account')?>
              <option value="forward" <?php if($_REQUEST["deliverymode"] == "forward"){ echo "SELECTED";} ?>><?=$LANG->_('Forward account')?>
            </select>
          </td>
        </tr>

<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Do auto reply')?></td>
          <td><input type="checkbox" name="reply" value="ok" <?php if($_REQUEST["reply"] =="ok"){ echo "checked";} ?>><?=$LANG->_('Send an auto reply')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Text for replying')?></td>
          <td valign="center"><?php echo pql_format_error_span($error["mailreplytext"]); ?> <textarea name="mailreplytext" cols="40" rows="10"><?=$_REQUEST["mailreplytext"]?></textarea></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <table>
            <td><img src="images/info.png" width="16" height="16" border="0" align="left"></td>
            <td>
              <?=$LANG->_('If send auto reply is enabled, QmailLDAP will first store the mail in the users mailbox, THEN send the reply')?>.
            </td>
          </table>
        </tr>
      </th>
    </table>

<?php if($_SESSION["SINGLE_USER"]) { ?>
    <input type="hidden" name="deliverymode" value="<?=$_REQUEST["deliverymode"]?>">
<?php } ?>
    <input type="hidden" name="submit"       value="1">
    <input type="hidden" name="attrib"       value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue"     value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"       value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"       value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"         value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="view"         value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}

function attribute_save($type) {
    global $_pql, $LANG;
	
	switch($_POST["deliverymode"]) {
	  case "forward":
		$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "forwardonly";
		$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "nombox";
		break;

	  case "local":
		$entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "localdelivery";
	}
	
	if($_POST["reply"] == "ok")
	  $entry[pql_get_define("PQL_GLOB_ATTR_MODE")][] = "reply";

	if (!empty($_POST["mailreplytext"])) 
	  $entry[pql_get_define("PQL_GLOB_ATTR_REPLYTEXT")] = preg_replace('/\\\/', "", $_POST["mailreplytext"]);
	else {
		// Check if the entry in database is empty. 
		// If not, we have choosen to DELETE the value
		$mrt_db = pql_get_attribute($_pql->ldap_linkid, $_POST["user"], pql_get_define("PQL_GLOB_ATTR_REPLYTEXT"));
		if($mrt_db[0])
		  pql_replace_attribute($_pql->ldap_linkid, $_POST["user"], pql_get_define("PQL_GLOB_ATTR_REPLYTEXT"));
	}
	
	switch($type) {
	  case "fulldomain":
		$users = pql_user_get($_pql->ldap_linkid, $_POST["domain"]);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_attribute($_pql->ldap_linkid, $_POST["user"], '', $entry);
			
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
		if(pql_replace_attribute($_pql->ldap_linkid, $_POST["user"], '', $entry)) 
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
