<?php
// control attribute plugin for
// pbsport (max number of bytes in message)
//
// $Id: attrib.control.pbsport.inc,v 2.12 2007-02-15 12:07:50 turbo Exp $

// {{{ pbsport_check(type)
function pbsport_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ pbsport_init(host)
// init values
function pbsport_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["pbsport"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_ATTR_PBS_PORT"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["pbsport"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_PBS_PORT"));

	if(@is_null($_REQUEST["pbsport"]))
	  return true;
}
// }}}


// {{{ pbsport_print_view(host)
function pbsport_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbsport_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">pbsport (<?php echo $LANG->_('POP before SMTP server port')?>)
<?php $class=pql_format_table(0); ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Port number</td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["pbsport"]) { echo $_REQUEST["pbsport"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_PBS_PORT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_PBS_PORT")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbsport_print_form(void)
function pbsport_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">pbsport (<?php echo $LANG->_('POP before SMTP server port')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Port')?></td>
          <td><?php echo pql_format_error_span($error["pbsport"]); ?><input type="text" name="pbsport" value="<?php echo $_REQUEST["pbsport"]?>"></td>
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

// {{{ pbsport_save(type, host)
function pbsport_save($type, $host)  {
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
									 pql_get_define("PQL_ATTR_PBS_PORT"), '', $_REQUEST["pbsport"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_PBS_PORT"), 1, $_REQUEST["pbsport"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbsport options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbsport options')))
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


// {{{ pbsport_help(void)
function pbsport_help() {
	global $LANG;
?>
    The port where all pbsdbd servers are listening on.
    This file if used is needed by all pbs tools and has to be in sync on all
    cluster machines.
<?php
}
// }}}

// {{{ pbsport_help_cr(void)
function pbsport_help_cr() {
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
