<?php
// control attribute plugin for
// pbscachesize (max number of bytes in message)
//
// $Id: attrib.control.pbscachesize.inc,v 2.12 2007-02-15 12:07:39 turbo Exp $

// {{{ pbscachesize_check(type)
function pbscachesize_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ pbscachesize_init(host)
// init values
function pbscachesize_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["pbscachesize"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													pql_get_define("PQL_ATTR_PBS_CACHESIZE"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["pbscachesize"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_PBS_CACHESIZE"));

	if(@is_null($_REQUEST["pbscachesize"]))
	  return true;
}
// }}}


// {{{ pbscachesize_print_view(host)
function pbscachesize_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbscachesize_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">pbscachesize (<?php echo $LANG->_('POP before SMTP cache size')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Bytes')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["pbscachesize"]) { echo $_REQUEST["pbscachesize"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_PBS_CACHESIZE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_PBS_CACHESIZE")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbscachesize_print_form(void)
function pbscachesize_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">pbscachesize (<?php echo $LANG->_('POP before SMTP cache size')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Bytes')?></td>
          <td><?php echo pql_format_error_span($error["pbscachesize"]); ?><input type="text" name="pbscachesize" value="<?php echo $_REQUEST["pbscachesize"]?>"></td>
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

// {{{ pbscachesize_save(type, host)
function pbscachesize_save($type, $host)  {
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
									 pql_get_define("PQL_ATTR_PBS_CACHESIZE"), '', $_REQUEST["pbscachesize"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_PBS_CACHESIZE"), 1, $_REQUEST["pbscachesize"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbscachesize options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbscachesize options')))
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


// {{{ pbscachesize_help(void)
function pbscachesize_help() {
	global $LANG;
?>
    Size in bytes used for the cache. Normally you do not need to set this
    because the default value is big enough (1Mb).
<?php
}
// }}}

// {{{ pbscachesize_help_cr(void)
function pbscachesize_help_cr() {
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
