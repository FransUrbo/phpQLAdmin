<?php
// attribute plugin for
// deliverymode (delivery preferences)
// $Id: attrib.deliverymode.inc,v 2.33 2007-02-26 11:00:46 turbo Exp $

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
	$mode = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_MODE"));

	// delivery profile
	if(is_array($mode)) {
		foreach($mode as $m)
		  $_REQUEST[$m] = 1;

		if(in_array("forwardonly", $mode) or in_array("nolocal", $mode))
		  $_REQUEST["deliverymode"] = "forward";
	} else
	  $_REQUEST["deliverymode"] = "local";

	// reply / mailreplytext
	if(is_array($mode) and in_array("reply", $mode))
	  $_REQUEST["reply"] = "ok";
	
	if($_SESSION["NEW_STYLE_QUOTA"] and is_array($mode) and in_array("noprogram", $mode)) {
		// no program deliveries
		$_REQUEST["noprogram"] = "ok";
	}
	
	$_REQUEST["mailreplytext"] = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_REPLYTEXT"));
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $error, $LANG, $_pql;

	// {{{ Retreive old value(s)
	$deliverymode = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_MODE"));
	if(!is_array($deliverymode))
	  $deliverymode = array($deliverymode);

	if(in_array('nolocal', $deliverymode) and !$_REQUEST["programpath"])
	  $_REQUEST["programpath"] = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_PROGRAM"));

	if(in_array('reply', $deliverymode) and !$_REQUEST["mailreplytext"])
	  $_REQUEST["mailreplytext"] = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_REPLYTEXT"));
	// }}}
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Delivery mode')?></th>
<?php if(!$_SESSION["SINGLE_USER"]) {
		if($_SESSION["NEW_STYLE_QUOTA"]) {
			// LDAP_MODE (default: "deliveryMode")
			// Multi field entries of these keywords:
			//   - (normal)		Put message into maildir/mbox, plus forward and program delivery
			//   - noforward	Do not forward (ignores forwarding entries in ldap and .qmail)
			//	 - nolocal		Do not put message into maildir/mbox (ignores also .qmail)
			//	 - noprogram	Do not do program deliveries (ignores deliveryprogrampath, .qmail)
			//   - reply		Send an auto_reply mail with text from mailReplyText
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Delivery mode')?></td>
          <td>
            <?=pql_format_error_span($error["deliverymode"])?>
            <input type="checkbox" name="nolocal"   <?php if($_REQUEST["nolocal"] or in_array('nolocal', $deliverymode)) { echo "CHECKED"; } ?>><?=$LANG->_('Do not deliver to local mail box')?><br>
            <input type="checkbox" name="noforward" <?php if($_REQUEST["noforward"] or in_array('noforward', $deliverymode)) { echo "CHECKED"; } ?>><?=$LANG->_('Do not forward mail to other account/address')?><br>
            <input type="radio" name="delivery_type" value="noprogram" <?php if($_REQUEST["noprogram"] or in_array('noprogram', $deliverymode)) { echo "CHECKED"; } ?>><?=$LANG->_('No program deliveries')?><br>
<?php if($_REQUEST["programpath"]) { ?>
            <input type="radio" name="delivery_type" value="program"><input type="text" name="programpath" value="<?=$_REQUEST["programpath"]?>">
<?php } else { ?>
            <input type="radio" name="delivery_type" value="program"><input type="text" name="programpath">
<?php } ?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" border="0"></td>
          <td><?=$LANG->_('If all unset, then do normal delivery')?></td>
        </tr>
<?php	} else { ?>
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
<?php	} 
	  }
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Send automatic reply')?></td>
          <td><input type="checkbox" name="reply" value="ok" <?php if(($_REQUEST["reply"] == "ok") or in_array('reply', $deliverymode)) { echo "CHECKED"; } ?>><?=$LANG->_('Send an auto reply')?></td>
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
              <?=$LANG->_('If the \'No program deliveries\' is set, QmailLDAP will ignore <i>deliveryProgramPath</i> and <i>.qmail</i> files')?>.
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
    <input type="hidden" name="domain"       value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"       value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"         value="<?=urlencode($_REQUEST["user"])?>">
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
	echo "attribute_save($type)<br>"; printr($_REQUEST);

	// Get old values
	$user = $_pql->search($_REQUEST["user"], 'objectClass=*', 'BASE');

	// Initial deliveryMode values. We remove from this what is not supposed to be there...
	$modes = $user[pql_get_define("PQL_ATTR_MODE")];

	if($_SESSION["NEW_STYLE_QUOTA"]) {
	  // {{{ Go through nolocal and noforward
	  $values = array('nolocal', 'noforward');
	  foreach($values as $value) {
		if($_REQUEST[$value])
		  $modes[] = $value;
		elseif(in_array($value, $modes)) {
		  // Not choosen, and it's in the users object - remove that!
		  for($i=0; $modes[$i]; $i++) {
			if($modes[$i] == $value)
			  // This is the value - remove it!
			  unset($modes[$i]);
		  }
		}
	  }
	  $modes = pql_uniq($modes); // Fixup the array (so it starts from 0)
	  // }}}

	  // {{{ Go through program/noprogram
	  if($_REQUEST["delivery_type"] == 'noprogram') {
		// no program deliveries
		$modes[] = "noprogram";

		// Don't do this - it's perfectly safe to have a deliveryProgramPath if 'noprogram' is set!
		//if($user[pql_get_define("PQL_ATTR_PROGRAM")])
		  // There is a value for deliveryProgramPath - remove that
		  //$entry[pql_get_define("PQL_ATTR_PROGRAM")] = array();
	  } elseif(($_REQUEST["delivery_type"] == 'program') and $_REQUEST["programpath"]) {
		if(( is_array($user[pql_get_define("PQL_ATTR_PROGRAM")]) and !in_array($_REQUEST["programpath"], $user[pql_get_define("PQL_ATTR_PROGRAM")])) or
		   (!is_array($user[pql_get_define("PQL_ATTR_PROGRAM")]) and $user[pql_get_define("PQL_ATTR_PROGRAM")] != $_REQUEST["programpath"]))
		  // delivery_type=program, program path is set and the program is not already in the users object - add the program
		  $entry[pql_get_define("PQL_ATTR_PROGRAM")] = $_REQUEST["programpath"];

		if(in_array('noprogram', $modes)) {
		  // We've specified to do program deliveries, but the 'noprogram' value is set - remove it!
		  for($i=0; $modes[$i]; $i++) {
			if($modes[$i] == 'noprogram')
			  // This is the value - remove it!
			  unset($modes[$i]);
		  }
		}
	  }
	  $modes = pql_uniq($modes); // Fixup the array (so it starts from 0)
	  // }}}
	} else {
	  // {{{ Old style quota/delivery mode
	  switch($_REQUEST["deliverymode"]) {
	  case "forward":
		$entry[pql_get_define("PQL_ATTR_MODE")][] = "forwardonly";
		$entry[pql_get_define("PQL_ATTR_MODE")][] = "nombox";
		break;
		
	  case "local":
		$entry[pql_get_define("PQL_ATTR_MODE")][] = "localdelivery";
	  }
	  // }}}
	}
	
	if($_REQUEST["reply"] == "ok")
	  $modes[] = "reply";

	$entry[pql_get_define("PQL_ATTR_MODE")] = $modes;
	
	if(!empty($_REQUEST["mailreplytext"]) and $user[pql_get_define("PQL_ATTR_REPLYTEXT")]) {
		$tmp = preg_replace('/\\\/', "", $_REQUEST["mailreplytext"]);
		$entry[pql_get_define("PQL_ATTR_REPLYTEXT")] = pql_maybe_encode($tmp, 'mailreplytext', $_pql->ldap_linkid);
	} elseif(($type != "fulldomain") and $user[pql_get_define("PQL_ATTR_REPLYTEXT")])
	  // Delete the old value from the database
	  pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_REPLYTEXT"), '', '');

	switch($type) {
	  case "fulldomain":
		// Create a user search filter (only look for mail users - !?!?).
		$filter  = "(&(".pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*)(";
		$filter .= pql_get_define("PQL_ATTR_MAIL")."=*))";

		// Retreive all users in this branch/domain.
	    $users   = $_pql->get_dn($_REQUEST["domain"], $filter);
		if(is_array($users)) {
			foreach($users as $user)
			  $return[] = pql_modify_attribute($user, '', '', $entry);

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
		if(!empty($entry)) {
		  // Got changes to make - make them.
		  if(pql_modify_attribute($_REQUEST["user"], '', '', $entry))
			$msg = pql_complete_constant($LANG->_('successfully changed %what%'),
										 array('what' => $LANG->_('delivery mode')));
		  else
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('delivery mode'))) . ": " . pql_format_error(0);
		} else {
		  $msg = $LANG->_('No changes made to delivery mode');
		}

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
