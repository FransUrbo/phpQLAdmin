<?php
// attribute plugin for
// deliverymode (delivery preferences)
// $Id: attrib.deliverymode.inc,v 2.21.6.2 2004-07-07 12:25:51 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;

	// if reply is selected, mailreplytext must not be null
	if(!empty($_REQUEST["reply"]) and empty($_REQUEST["mailreplytext"])) {
		$error["mailreplytext"] = $LANG->_('Missing');
		return false;
	}

	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    global $_pql;

	// initialize values directly from ldap
	$mode = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_MODE"));

	// delivery profile
	if(is_array($mode) and
	   (in_array("forwardonly", $mode) or in_array("nolocal", $mode)))
	  $_REQUEST["deliverymode"] = "forward";
	else
	  $_REQUEST["deliverymode"] = "local";

	// reply / mailreplytext
	if(is_array($mode) and in_array("reply", $mode))
	  $_REQUEST["reply"] = "ok";
	
	if($_SESSION["NEW_STYLE_QUOTA"] and is_array($mode) and in_array("noprogram", $mode)) {
		// no program deliveries
		$_REQUEST["noprogram"] = "ok";
	}
	
	$mailreplytext = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_REPLYTEXT"));
	$_REQUEST["mailreplytext"] = $mailreplytext[0];
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $url, $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Delivery mode')?></th>
<?php if(!$_SESSION["SINGLE_USER"]) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Delivery mode')?></td>
          <td>
            <?=pql_format_error_span($error["deliverymode"])?>
            <select name="deliverymode">
              <option value="local" <?php if($_REQUEST["deliverymode"] == "local") { echo "SELECTED"; } ?>><?=$LANG->_('Deliver mail to local mail box')?>
              <option value="forward" <?php if($_REQUEST["deliverymode"] == "forward") { echo "SELECTED"; } ?>><?=$LANG->_('Forward mail to other account/address')?>
            </select>
          </td>
        </tr>

<?php	if($_SESSION["NEW_STYLE_QUOTA"]) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Program deliveries')?></td>
          <td><input type="checkbox" name="noprogram" value="ok" <?php if($_REQUEST["noprogram"] == "ok") { echo "CHECKED"; } ?>><?=$LANG->_('No program deliveries')?></td>
        </tr>

<?php	}
	  }
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Send automatic reply')?></td>
          <td><input type="checkbox" name="reply" value="ok" <?php if($_REQUEST["reply"] == "ok"){ echo "checked";} ?>><?=$LANG->_('Send an auto reply')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Text for replying')?></td>
          <td valign="center"><?php echo pql_format_error_span($error["mailreplytext"]); ?> <textarea name="mailreplytext" cols="40" rows="10"><?=$_REQUEST["mailreplytext"]?></textarea></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <table>
            <td><img src="images/info.png" width="16" height="16" border="0" align="left"></td>
            <td>
              <?=$LANG->_('If send auto reply is enabled, QmailLDAP will first store the mail in the users mailbox (or forward the mail to other account/address), <i>THEN</i> send the reply')?>.
            </td>
          </table>
        </tr>
<?php	if($_SESSION["NEW_STYLE_QUOTA"]) { ?>

        <tr class="<?php pql_format_table(); ?>">
          <table>
            <td><img src="images/info.png" width="16" height="16" border="0" align="left"></td>
            <td>
              <?=$LANG->_('If the \'No program deliveries\' is set, QmailLDAP will ignore <i>deliveryProgramPath</i> and <i>.qmail</i>')?>.
            </td>
          </table>
        </tr>
<?php	} ?>
      </th>
    </table>

<?php if($_SESSION["SINGLE_USER"]) { ?>
    <input type="hidden" name="deliverymode" value="<?=$_REQUEST["deliverymode"]?>">
<?php } ?>
    <input type="hidden" name="submit"       value="1">
    <input type="hidden" name="attrib"       value="<?=$_REQUEST["attrib"]?>">
	<!-- DLW: oldvalue should be 3 values for the 3 fields. -->
    <input type="hidden" name="oldvalue"     value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"       value="<?=$url["domain"]?>">
    <input type="hidden" name="rootdn"       value="<?=$url["rootdn"]?>">
    <input type="hidden" name="user"         value="<?=$url["user"]?>">
    <input type="hidden" name="view"         value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;
	$entry = array();

	// If running with newer QmailLDAP (>= 20030801), then
	// 'nolocal' and 'noforward' are mutualy exclusive.
	switch($_REQUEST["deliverymode"]) {
	  case "forward":
		if($_SESSION["NEW_STYLE_QUOTA"]) {
			$entry[pql_get_define("PQL_ATTR_MODE")][] = "nolocal";
		} else {
			$entry[pql_get_define("PQL_ATTR_MODE")][] = "forwardonly";
			$entry[pql_get_define("PQL_ATTR_MODE")][] = "nombox";
		}
		break;
		
	  case "local":
		if($_SESSION["NEW_STYLE_QUOTA"]) {
			$entry[pql_get_define("PQL_ATTR_MODE")][] = "noforward";
		} else {
			$entry[pql_get_define("PQL_ATTR_MODE")][] = "localdelivery";
		}
	}
	
	if($_SESSION["NEW_STYLE_QUOTA"] and ($_REQUEST["noprogram"] == "ok")) {
		// no program deliveries
		$entry[pql_get_define("PQL_ATTR_MODE")][] = "noprogram";
	}

	if($_REQUEST["reply"] == "ok")
	  $entry[pql_get_define("PQL_ATTR_MODE")][] = "reply";
	
	if (!empty($_REQUEST["mailreplytext"])) {
		$tmp = preg_replace('/\\\/', "", $_REQUEST["mailreplytext"]);
		$entry[pql_get_define("PQL_ATTR_REPLYTEXT")] = pql_maybe_encode($tmp, 'mailreplytext', $_pql->ldap_linkid);
	} else {
		// Check if the entry in database is empty. 
		// If not, we have choosen to DELETE the value
		$mrt_db = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_REPLYTEXT"));
		if($mrt_db[0])
		  pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_REPLYTEXT"));
	}

	switch($type) {
	  case "fulldomain":
		$users = pql_user_get($_pql->ldap_linkid, $_REQUEST["domain"]);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_replace_attribute($_pql->ldap_linkid, $user, '', $entry);

			if(in_array(false, $return))
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('delivery mode'))) . ": " . pql_format_error(0);
			else
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('delivery mode')));
			
			attribute_forward($msg);
		}
		break;

	  case "modify":
		if(pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], '', $entry)) 
		  $msg = pql_complete_constant($LANG->_('successfully changed %what%'),
									   array('what' => $LANG->_('delivery mode')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('delivery mode'))) . ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
