<?php
// control attribute plugin for
// dirmaker (default prefix for mailbox paths)
//
// $Id: attrib.control.dirmaker.inc,v 2.17 2005-01-29 18:46:57 turbo Exp $

// {{{ dirmaker_check(type)
function dirmaker_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["dirmaker"] == "") {
		$error["dirmaker"] = $LANG->_('Missing');
		return false;
	}

	// trailing slash ?
	if(!preg_match ("/^(\/)/", $_REQUEST["dirmaker"])) {
		$error["dirmaker"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ dirmaker_init(host)
// init values
function dirmaker_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["dirmaker"] = pql_get_attribute($_pql_control->ldap_linkid,
											  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											  pql_get_define("PQL_ATTR_DIRMAKER"));

	if(is_null($_REQUEST["dirmaker"]))
	  return true;
}
// }}}


// {{{ dirmaker_print_view(host)
function dirmaker_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	dirmaker_init($host);

	// set empty field to "not set"
	if($_REQUEST["dirmaker"] == "")
	  $_REQUEST["dirmaker"] = $LANG->_('Not set');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">dirmaker (<?=$LANG->_('Path to homedir creating script')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Path')?></td>
        <td><?=$_REQUEST["dirmaker"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=dirmaker"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ dirmaker_print_form(void)
function dirmaker_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">dirmaker (<?=$LANG->_('Path to homedir creating script')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Path')?></td>
          <td><?php echo pql_format_error_span($error["dirmaker"]); ?><input type="text" name="dirmaker" value="<?=$_REQUEST["dirmaker"]?>"></td>
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

// {{{ dirmaker_save(type, host)
function dirmaker_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_DIRMAKER"), '', $_REQUEST["dirmaker"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('dirmaker options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('dirmaker')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ dirmaker_help(void)
function dirmaker_help() {
	global $LANG;
?>
	If you compiled the autohomedirmake-feature, this contains the FULL path to your script which
	creates missing homedirs. The script is executed under the affected user's uid/gid, so if your
	homedirs are in /home and the homdir for joe (system uid joe) should be created, joe MUST have
	write permissions to /home. This feature is most usefull in virtual user environments where all
	users are mapped to an single system uid/gid pair, let's say virtual/virtual. Then only virtual
	needs write permissions in /home. The script gets the path for the to be created homedir as first
	parameter and aliasempty as second one. A sample script:
	<br>
	<br>
	<pre>
#!/bin/sh
mkdir -m 700 -p $1
	</pre>
<?php
}
// }}}

// {{{ badmailfrom_help_cr(void)
function badmailfrom_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
