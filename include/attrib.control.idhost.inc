<?php
// control attribute plugin for
// idhost (host name for message id's)
//
// $Id: attrib.control.idhost.inc,v 2.16 2004-03-11 16:31:54 turbo Exp $

// {{{ idhost_check(type)
function idhost_check($type) {
	global $error, $LANG;

	// validity
	if($_REQUEST["idhost"] != "" and !pql_check_hostaddress($_REQUEST["idhost"])) {
		$error["idhost"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ idhost_init(host)
// init values
function idhost_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_IDHOST"));
	
	if(!is_null($value))
	  $_REQUEST["idhost"] = $value[0];
}
// }}}


// {{{ idhost_print_view(host)
function idhost_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	idhost_init($host);

	// set empty field to default value
	if($_REQUEST["idhost"] == "")
	  $_REQUEST["idhost"] = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">idhost (<?=$LANG->_('Host name for Message-ID\'s')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Host')?></td>
        <td><?=pql_maybe_idna_decode($_REQUEST["idhost"])?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=idhost"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ idhost_print_form(void)
function idhost_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">idhost (<?=$LANG->_('Host name for Message-ID\'s')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["idhost"]); ?><input type="text" name="idhost" value="<?=$_REQUEST["idhost"]?>"> <?=$LANG->_('Default')?>: <?=$_REQUEST["mxhost"]?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ idhost_save(type, host)
function idhost_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_IDHOST"),
										 $_REQUEST["idhost"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('idhost options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('idhost options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ idhost_help(void)
function idhost_help() {
	global $LANG;
?>
	Current host name, for use solely in saying hello to
	the remote SMTP server.  Default:  me, if that is
	supplied; otherwise qmail-remote refuses to run.
<?php
}
// }}}

// {{{ idhost_help_cr(void)
function idhost_help_cr() {
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