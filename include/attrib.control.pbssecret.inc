<?php
// control attribute plugin for
// pbssecret (max number of bytes in message)
//
// $Id: attrib.control.pbssecret.inc,v 2.12 2007-02-15 12:07:53 turbo Exp $

// {{{ pbssecret_check(type)
function pbssecret_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ pbssecret_init(host)
// init values
function pbssecret_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["pbssecret"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												 pql_get_define("PQL_ATTR_PBS_SECRET"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["pbssecret"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_PBS_SECRET"));

	if(@is_null($_REQUEST["pbssecret"]))
	  return true;
}
// }}}


// {{{ pbssecret_print_view(host)
function pbssecret_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbssecret_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">pbssecret (<?=$LANG->_('POP before SMTP secret')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Secret')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["pbssecret"]) { echo $_REQUEST["pbssecret"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_PBS_SECRET")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_PBS_SECRET")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbssecret_print_form(void)
function pbssecret_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">pbssecret (<?=$LANG->_('POP before SMTP secret')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Secret')?></td>
          <td><?php echo pql_format_error_span($error["pbssecret"]); ?><input type="text" name="pbssecret" value="<?=$_REQUEST["pbssecret"]?>"></td>
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

// {{{ pbssecret_save(type, host)
function pbssecret_save($type, $host)  {
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
									 pql_get_define("PQL_ATTR_PBS_SECRET"), '', $_REQUEST["pbssecret"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_PBS_SECRET"), 1, $_REQUEST["pbssecret"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbssecret options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbssecret options')))
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


// {{{ pbssecret_help(void)
function pbssecret_help() {
	global $LANG;
?>
    Shared secret used by pbsadd and pbsdbd to authenticate the client.
    Required for pbsdbd and pbsadd.
<?php
}
// }}}

// {{{ pbssecret_help_cr(void)
function pbssecret_help_cr() {
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
