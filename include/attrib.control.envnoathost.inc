<?php
// control attribute plugin for
// envnoathost (default prefix for mailbox paths)
//
// $Id: attrib.control.envnoathost.inc,v 2.18 2005-01-29 18:46:57 turbo Exp $

// {{{ envnoathost_check(type)
function envnoathost_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["envnoathost"] == "") {
		$error["envnoathost"] = $LANG->_('Missing');
		return false;
	}

	// validity
	if(!pql_check_hostaddress($_REQUEST["envnoathost"])) {
		$error["envnoathost"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ envnoathost_init(host)
// init values
function envnoathost_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["envnoathost"] = pql_get_attribute($_pql_control->ldap_linkid,
												 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												 pql_get_define("PQL_ATTR_ENVNOATHOST"));

	if(is_null($_REQUEST["envnoathost"]))
	  return true;
}
// }}}


// {{{ envnoathost_print_view(host)
function envnoathost_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	envnoathost_init($host);

	// set empty field to default value
	if($_REQUEST["envnoathost"] == "")
	  $_REQUEST["envnoathost"] = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">envnoathost (<?=$LANG->_('Default domain for addresses without @')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Email')?></td>
        <td><?=pql_maybe_idna_decode($_REQUEST["envnoathost"])?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=envnoathost"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ envnoathost_print_form(void)
function envnoathost_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">envnoathost (<?=$LANG->_('Default domain for addresses without @')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Email')?></td>
          <td><?php echo pql_format_error_span($error["envnoathost"]); ?><input type="text" name="envnoathost" value="<?=$_REQUEST["envnoathost"]?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr>
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ envnoathost_save(type, host)
function envnoathost_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_ENVNOATHOST"), '', $_REQUEST["envnoathost"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('envnotathost options')));
		else 
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('envnoathost options')))
			. ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ envnoathost_help(void)
function envnoathost_help() {
	global $LANG;
?>
	Presumed domain name for addresses without @ signs.
	Default:  me, if that is supplied; otherwise the
	literal name envnoathost, which is probably not what
	you want.  If qmail-send sees an envelope recipient
	address without an @ sign, it appends @envnoathost.
<?php
}
// }}}

// {{{ envnotathost_help_cr(void)
function envnotathost_help_cr() {
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
