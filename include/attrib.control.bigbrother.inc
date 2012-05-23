<?php
// control attribute plugin for
// bigbrother (max number of bytes in message)
//
// $Id: attrib.control.bigbrother.inc,v 2.13 2007-02-15 12:07:18 turbo Exp $

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
	if($host == "Global")
	  $_REQUEST["bigbrother"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												  pql_get_define("PQL_ATTR_BIGBROTHER"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["bigbrother"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_BIGBROTHER"));

	if(!@$_REQUEST["bigbrother"])
	  return true;
	elseif(!@is_array($_REQUEST["bigbrother"]))
	  $_REQUEST["bigbrother"] = array($_REQUEST["bigbrother"]);
}
// }}}


// {{{ bigbrother_print_view(host)
function bigbrother_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	bigbrother_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left">bigbrother (<?php echo $LANG->_('Global mail forwarding/surveillance')?>)
      <tr class="title">
        <td><?php echo $LANG->_('Mail sent to/from')?></td>
        <td><?php echo $LANG->_('Copy to')?></td>
        <td></td>
      </tr>
<?php
	if(!@is_array($_REQUEST["bigbrother"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td colspan="3"><?php echo $LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["bigbrother"] as $addresses) {
			$address = explode(':', $addresses);
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?php echo $address[0]?></td>
        <td><?php echo $address[1]?></td>
        <td align="right">
          <a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_BIGBROTHER")?>&type=del&submit=1&<?php echo pql_get_define("PQL_ATTR_BIGBROTHER")?>=<?php echo urldecode($addresses)?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Remove address')?>"></a>
        </td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td colspan="3"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_BIGBROTHER")?>"><img src="images/edit.png" width="12" height="12" border="0"><?php echo $LANG->_('Add address')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ bigbrother_print_form(void)
function bigbrother_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["bigbrother"]); $i++) { ?>
    <input type="hidden" name="bigbrother[<?php echo $i?>]" value="<?php echo $_REQUEST["bigbrother"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">bigbrother (<?php echo $LANG->_('Global mail forwarding/surveillance')?>)</th>
        <tr class="title">
          <td><?php echo $LANG->_('Mail sent to/from')?></td>
          <td><?php echo $LANG->_('Copy to')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><input type="text" name="bbmail_to"></td>
          <td><input type="text" name="bbmail_cpy"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ bigbrother_save(type, host)
function bigbrother_save($type, $host)  {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		for($i=0; $i < count($_REQUEST["bigbrother"]); $i++)
		  $entry[] = $_REQUEST["bigbrother"][$i];
		$entry[] = $_REQUEST["bbmail_to"].":".$_REQUEST["bbmail_cpy"];

		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_BIGBROTHER"), '', $entry))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_BIGBROTHER"), 1, $entry))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('bigbrother options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bigbrother options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		// {{{ delete value
		if(pql_modify_attribute($host,
								pql_get_define("PQL_ATTR_BIGBROTHER"), $_REQUEST["bigbrother"], ''))
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
