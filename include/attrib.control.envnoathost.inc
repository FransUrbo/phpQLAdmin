<?php
// control attribute plugin for
// envnoathost (default prefix for mailbox paths)
//
// $Id: attrib.control.envnoathost.inc,v 2.10 2003-08-15 08:06:17 turbo Exp $

function envnoathost_check($type) {
	global $error, $envnoathost, $view, $LANG;

	// missing
	if($envnoathost == "") {
		$error["envnoathost"] = $LANG->_('Missing');
		return false;
	}

	// validity
	if(!check_hostaddress($envnoathost)) {
		$error["envnoathost"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}

// init values
function envnoathost_init($host) {
	global $_pql_control, $envnoathost, $view;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_ENVNOATHOST"));

	if(!is_null($value))
	  $envnoathost = $value[0];
}

function envnoathost_print_view($host) {
	global $_pql_control, $envnoathost, $view, $LANG;

	// init data
	envnoathost_init($host);

	// set empty field to default value
	if($envnoathost == "")
	  $envnoathost = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">envnoathost (<?=$LANG->_('Default domain for addresses without @')?>)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Email')?></td>
        <td><?=$envnoathost?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=envnoathost"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function envnoathost_print_form() {
	global $attrib, $envnoathost, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">envnoathost (<?=$LANG->_('Default domain for addresses without @')?>)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Email')?></td>
          <td><?php echo format_error($error["envnoathost"]); ?><input type="text" name="envnoathost" value="<?=$envnoathost?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr>
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function envnoathost_save($type, $host) {
	global $_pql_control, $attrib, $envnoathost, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_ENVNOATHOST"), $envnoathost))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('envnotathost options')));
		else 
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('envnoathost options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

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


function envnotathost_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
