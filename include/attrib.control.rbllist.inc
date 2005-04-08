<?php
// control attribute plugin for
// rbllist (max number of bytes in message)
//
// $Id: attrib.control.rbllist.inc,v 2.7 2005-01-29 18:46:57 turbo Exp $

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
	$_REQUEST["rbllist"] = pql_get_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_ATTR_RBLLIST"));

	if(is_null($_REQUEST["rbllist"]))
	  return true;
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
      </tr>
<?php
	if(!is_array($_REQUEST["rbllist"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="4"><?=$LANG->_('Not set')?></td>
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
        <td><?php for($i=3; $line[$i]; $i++) { echo $line[$i]." "; } ?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td colspan="4"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=rbllist"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ rbllist_print_form(void)
function rbllist_print_form() {
	global $error, $LANG;

//	if(($_REQUEST["rblcount"] < count($_REQUEST["rbllist"]) - 1) and count($_REQUEST["rblserver"]) != 0)
//	  $_REQUEST["rblcount"] = count($_REQUEST["rbllist"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="4" align="left">rbllist (<?=$LANG->_('Realtime Blackhole List')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('RBL Address')?></td>
          <td><?=$LANG->_('Action')?></td>
          <td><?=$LANG->_('Match on')?></td>
          <td><?=$LANG->_('Message')?></td>
        </tr>

<?php if(is_array($_REQUEST["rbllist"])) { 
		for($i=0; $_REQUEST["rbllist"][$i]; $i++) {
			$line = split(' ', $_REQUEST["rbllist"][$i]);
?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["rbllist_$i"]); ?><input type="text" name="rblserver[<?=$i?>]" value="<?=$line[0]?>"></td>
          <td>
            <?php echo pql_format_error_span($error["rbllist_$i"]); ?>
            <select name="rblaction[<?=$i?>]">
              <option value="reject" <?php if($line[1] == "reject") { echo "SELECTED"; } ?>><?=$LANG->_('Reject mail')?></option>
              <option value="addheader" <?php if($line[1] == "addheader") { echo "SELECTED"; } ?>><?=$LANG->_('Add header')?></option>
            </select>
          </td>
          <td><?php echo pql_format_error_span($error["rbllist_$i"]); ?><input type="text" name="rblmatch[<?=$i?>]" value="<?=$line[2]?>" size="10"></td>
          <td><?php echo pql_format_error_span($error["rbllist_$i"]); ?><input type="text" name="rblmessage[<?=$i?>]" value="<?php for($j=3; $line[$j]; $j++) { echo $line[$j]." "; } ?>" size="40"></td>
        </tr>

<?php 	}
      } else {
		  $i=0;
	  }
?>
        <tr class="<?php pql_format_table(); ?>">
          <td><input type="text" name="rblserver[<?=$i?>]"></td>
          <td>
            <?php echo pql_format_error_span($error["rbllist_<?=$i?>"]); ?>
            <select name="rblaction[<?=$i?>]">
              <option value="reject"><?=$LANG->_('Reject mail')?></option>
              <option value="addheader"><?=$LANG->_('Add header')?></option>
            </select>
          </td>
          <td><input type="text" name="rblmatch[<?=$i?>]" size="10"></td>
          <td><input type="text" name="rblmessage[<?=$i?>]" size="40"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="4"><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&rblcount=<?=($_REQUEST["rblcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td colspan="4"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a RBL server, remove the value and save')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
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

	switch($type) {
	  case "modify":
	  case "add":
		// save entries

		for($i=0; $_REQUEST["rblserver"][$i]; $i++)
		  $entry[] = $_REQUEST["rblserver"][$i]." ".$_REQUEST["rblaction"][$i]." ".$_REQUEST["rblmatch"][$i]." ".$_REQUEST["rblmessage"][$i];

		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,". $_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_RBLLIST"), '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('rbllist options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('rbllist options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
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
