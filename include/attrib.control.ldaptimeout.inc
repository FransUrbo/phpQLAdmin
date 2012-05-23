<?php
// control attribute plugin for
// ldaptimeout (max number of bytes in message)
//
// $Id: attrib.control.ldaptimeout.inc,v 2.12 2007-02-15 12:07:31 turbo Exp $

// {{{ ldaptimeout_check(type)
function ldaptimeout_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ ldaptimeout_init(host)
// init values
function ldaptimeout_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["ldaptimeout"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_LDAPTIMEOUT"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["ldaptimeout"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_LDAPTIMEOUT"));

	if(@is_null($_REQUEST["ldaptimeout"]))
	  return true;
}
// }}}


// {{{ ldaptimeout_print_view(host)
function ldaptimeout_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldaptimeout_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldaptimeout (<?php echo $LANG->_('LDAP Timeout')?>)
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Seconds')?></td>
<?php
	if(!@$_REQUEST["ldaptimeout"]) {
?>
        <td><i><?php echo $LANG->_('Not set')?></i></td>
<?php
	} else {
?>
        <td><?php echo $_REQUEST["ldaptimeout"]." ".$LANG->_('seconds')?></td>
<?php
	} // end if is_array
?>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=ldaptimeout"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ ldaptimeout_print_form(void)
function ldaptimeout_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldaptimeout (<?php echo $LANG->_('LDAP Timeout')?>)</th>
        <tr class="title">
          <td><?php echo $LANG->_('Seconds')?></td>
          <td><?php echo pql_format_error_span($error["ldaptimeout_0"]); ?><input type="text" name="ldaptimeout" value="<?php echo $_REQUEST["ldaptimeout"]?>"></td>
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

// {{{ ldaptimeout_save(type, host)
function ldaptimeout_save($type, $host)  {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_LDAPTIMEOUT"), '', $_REQUEST["ldaptimeout"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_LDAPTIMEOUT"), 1, $_REQUEST["ldaptimeout"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldaptimeout options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldaptimeout options')))
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


// {{{ ldaptimeout_help(void)
function ldaptimeout_help() {
	global $LANG;
?>
    The time the ldap search waits for a response from the ldap server
    Default: 30 seconds
<?php
}
// }}}

// {{{ ldaptimeout_help_cr(void)
function ldaptimeout_help_cr() {
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
