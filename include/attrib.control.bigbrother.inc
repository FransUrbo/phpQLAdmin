<?php
// control attribute plugin for
// bigbrother (max number of bytes in message)
//
// $Id: attrib.control.bigbrother.inc,v 2.6 2004-10-19 10:40:40 turbo Exp $

// {{{ bigbrother_check(type)
function bigbrother_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ bigbrother_init(host)
// init values
function bigbrother_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["bigbrother"] = pql_get_attribute($_pql_control->ldap_linkid,
												pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												pql_get_define("PQL_ATTR_BIGBROTHER"));

	if(is_null($_REQUEST["bigbrother"]))
	  return true;
}
// }}}


// {{{ bigbrother_print_view(host)
function bigbrother_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	bigbrother_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">bigbrother (<?=$LANG->_('Global mail forwarding/surveillance')?>)
      <tr class="title">
        <td><?=$LANG->_('Mail sent to/from')?></td>
        <td><?=$LANG->_('Copy to')?></td>
      </tr>
<?php
	if(!is_array($_REQUEST["bigbrother"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="2"><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["bigbrother"] as $addresses) {
			$address = split(':', $addresses);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$address[0]?></td>
        <td><?=$address[1]?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=bigbrother"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ bigbrother_print_form(void)
function bigbrother_print_form() {
	global $error, $LANG;

//	if(($_REQUEST["bbcount"] < count($_REQUEST["bigbrother"]) - 1) and count($_REQUEST["bbmail"]) != 0)
//	  $_REQUEST["bbcount"] = count($_REQUEST["bigbrother"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">bigbrother (<?=$LANG->_('Global mail forwarding/surveillance')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Mail sent to/from')?></td>
          <td><?=$LANG->_('Copy to')?></td>
        </tr>

<?php if(is_array($_REQUEST["bigbrother"])) {
		for($i=0; $_REQUEST["bigbrother"][$i]; $i++) {
			$line = split(':', $_REQUEST["bigbrother"][$i]);
?>
        <tr class="<?php pql_format_table(); ?>">
          <td><input type="text" name="bbmail_to[<?=$i?>]" value="<?=$line[0]?>"></td>
          <td><input type="text" name="bbmail_cpy[<?=$i?>]" value="<?=$line[1]?>"></td>
        </tr>

<?php	}
	  } else {
		  $i=0;
	  }
?>
        <tr class="<?php pql_format_table(); ?>">
          <td><input type="text" name="bbmail_to[0]"></td>
          <td><input type="text" name="bbmail_cpy[0]"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&bbcount=<?=($_REQUEST["bbcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete an address for surveillance, remove the value and save')?>.</td>
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

// {{{ bigbrother_save(type, host)
function bigbrother_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		for($i=0; $_REQUEST["bbmail_to"][$i] and $_REQUEST["bbmail_cpy"][$i]; $i++)
		  $entry[] = $_REQUEST["bbmail_to"][$i] . ":" . $_REQUEST["bbmail_cpy"][$i];

		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"], '', '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('bigbrother options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bigbrother options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ bigbrother_help(void)
function bigbrother_help() {
	global $LANG;
?>
    This file is only used if BIGBROTHER was defined in the Makefile.
    For ISP that need to implement some surveillance method because of some
    beloved authoroties (like here in switzerland), you can enable a per
    address queue extra feature. To automatically send a copy of all mail to and from
    badguy@badplace.ch to bigbrother@admin.ch you need to add followong line to
    ~control/bigbrother: <u>badguy@badplace.ch:bigbrother@admin.ch</u>.<br>
    The address <b>bigbrother@admin.ch</b> can be a local, ldap or remote address.<br>
    <b>NOTE</b>: An extra recipient will be added in qmail-send. If the bigbrother
          address causes a bounce the sender will see this bounce.
<?php
}
// }}}

// {{{ bigbrother_help_cr(void)
function bigbrother_help_cr() {
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
