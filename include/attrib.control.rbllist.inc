<?php
// control attribute plugin for
// rbllist (max number of bytes in message)
//
// $Id: attrib.control.rbllist.inc,v 2.10.4.1 2006-11-13 07:16:05 turbo Exp $

// {{{ rbllist_check(type)
function rbllist_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ rbllist_init(host)
// init values
function rbllist_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["rbllist"] = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_RBLLIST"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["rbllist"] = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_RBLLIST"));

	if(!@$_REQUEST["rbllist"])
	  return true;
	elseif(!@is_array($_REQUEST["rbllist"]))
	  $_REQUEST["rbllist"] = array($_REQUEST["rbllist"]);
}
// }}}


// {{{ rbllist_print_view(host)
function rbllist_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	rbllist_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="4" align="left">rbllist (<?=$LANG->_('Realtime Blackhole List')?>)</th>
      <tr class="title">
        <td><?=$LANG->_('RBL Address')?></td>
        <td><?=$LANG->_('Action')?></td>
        <td><?=$LANG->_('Match on')?></td>
        <td><?=$LANG->_('Message')?></td>
        <td></td>
      </tr>
<?php
	if(!@is_array($_REQUEST["rbllist"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="5"><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["rbllist"] as $server) {
			$line = split(' ', $server);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$line[0]?></td>
        <td><?=$line[1]?></td>
        <td><?=$line[2]?></td>
        <td><?php for($i=3; $i < count($line); $i++) { echo $line[$i]." "; } ?></td>
        <td align="right">
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_RBLLIST")?>&type=del&submit=1&<?=pql_get_define("PQL_ATTR_RBLLIST")?>=<?=urldecode($server)?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Remove blocking list')?>"></a>
        </td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td colspan="5"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_RBLLIST")?>"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Add blocking list')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ rbllist_print_form(void)
function rbllist_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["rbllist"]); $i++) { ?>
    <input type="hidden" name="rbllist[<?=$i?>]" value="<?=$_REQUEST["rbllist"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="4" align="left">rbllist (<?=$LANG->_('Realtime Blackhole List')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('RBL Address')?></td>
          <td><?=$LANG->_('Action')?></td>
          <td><?=$LANG->_('Match on')?></td>
          <td><?=$LANG->_('Message')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><input type="text" name="rblserver"></td>
          <td>
            <?php echo pql_format_error_span($error["rbllist"]); ?>
            <select name="rblaction">
              <option value="reject"><?=$LANG->_('Reject mail')?></option>
              <option value="addheader"><?=$LANG->_('Add header')?></option>
            </select>
          </td>
          <td><input type="text" name="rblmatch" size="10"></td>
          <td><input type="text" name="rblmessage" size="40"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ rbllist_save(type, host)
function rbllist_save($type, $host)  {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		for($i=0; $i < count($_REQUEST["rbllist"]); $i++)
		  $entry[] = $_REQUEST["rbllist"][$i];
		$entry[] = $_REQUEST["rblserver"]." ".$_REQUEST["rblaction"]." ".$_REQUEST["rblmatch"]." ".$_REQUEST["rblmessage"];

		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($_pql_control->ldap_linkid, $host,
									 pql_get_define("PQL_ATTR_RBLLIST"), '', $entry))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid,
								  pql_get_define("PQL_ATTR_CN")."=$host,". $_SESSION["USER_SEARCH_DN_CTR"],
								  pql_get_define("PQL_ATTR_RBLLIST"), 1, $entry))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('rbllist options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('rbllist options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		// {{{ delete value
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_RBLLIST"), $_REQUEST["rbllist"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('Blocking list')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Blocking list')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ rbllist_help(void)
function rbllist_help() {
	global $LANG;
?>
    Rbllist contains a number of RBL's to check for the given senders IP address.
    The file consists of four tab or space separated fields.<br>
      <b>basedomain</b>: base domain address to lookup (e.g. relays.ordb.org)<br>
      <b>action</b>: one of addheader or reject.
              addheader will just create a X-RBL: header whereas
              reject will reject the smtp connection instantly with a 553 error.<br>
      <b>matchon</b>: any or IP-Address, if a IP-Address is specified the action is only
               taken if the returned address form basedomain is equal to
               IP-Address. With any all returned IP-Address will match.<br>
      <b>message</b>: message to be included in X-RBL: headers and 553 errors.
<?php
}
// }}}

// {{{ rbllist_help_cr(void)
function rbllist_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
