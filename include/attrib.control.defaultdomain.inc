<?php
// control attribute plugin for
// defaultdomain (default domain)
// defaulthost (default hostname)
//
// $Id: attrib.control.defaultdomain.inc,v 2.11 2004-01-01 12:35:33 turbo Exp $

function defaultdomain_check($type) {
	global $error, $defaultdomain, $defaulthost, $plusdomain, $view, $LANG;

	// check validity
	if($defaultdomain != "" and !pql_check_hostaddress($defaultdomain)) {
		$error["defaultdomain"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if($defaulthost != "" and !pql_check_hostaddress($defaulthost)) {
		$error["defaulthost"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if($plusdomain != "" and !pql_check_hostaddress($plusdomain)) {
		$error["plusdomain"] = $LANG->_('Invalid');
		$is_error = true;
	}

	if($is_error == false)
	  return true;
}

// init values
function defaultdomain_init($host) {
	global $_pql_control, $defaultdomain, $defaulthost, $plusdomain, $view;

	// fetch data from ldap server
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"),
					 pql_get_define("PQL_GLOB_ATTR_DEFAULTHOST"),
					 pql_get_define("PQL_GLOB_ATTR_PLUSDOMAIN"));

	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);

		if(!is_null($value))
		  $$attrib = $value[0];
	}
}

function defaultdomain_print_view($host) {
	global $_pql_control, $defaultdomain, $defaulthost, $plusdomain, $view, $LANG;

	// init data
	defaultdomain_init($host);

	// set empty fields to their default value
	if($defaultdomain == "")
	  $defaultdomain = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
	
	if($defaulthost == "")
	  $defaulthost = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
	
	if($plusdomain == "") 
	  $plusdomain = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">defaultdomain, defaulthost, plusdomain (<?=$LANG->_('Default domain / host')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Default domain')?></td>
        <td><?=$defaultdomain?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Default host')?></td>
        <td><?=$defaulthost?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Plus domain')?></td>
        <td><?=$plusdomain?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=defaultdomain"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function defaultdomain_print_form() {
	global $attrib, $defaultdomain, $defaulthost, $plusdomain, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">defaultdomain, defaulthost, plusdomain (<?=$LANG->_('Default domain / host')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default domain')?></td>
          <td><?php echo pql_format_error_span($error["defaultdomain"]); ?><input type="text" name="defaultdomain" value="<?=$defaultdomain?>"> <?=$LANG->_('Default')?>: <?=$host?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default host')?></td>
          <td><?php echo pql_format_error_span($error["defaulthost"]); ?><input type="text" name="defaulthost" value="<?=$defaulthost?>"> <?=$LANG->_('Default')?>: <?=$host?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Plus domain')?></td>
          <td><?php echo pql_format_error_span($error["plusdomain"]); ?><input type="text" name="plusdomain" value="<?=$plusdomain?>"> <?=$LANG->_('Default')?>: <?=$host?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr class="subtitle">
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

function defaultdomain_save($type, $host) {
	global $_pql_control, $attrib, $defaultdomain, $defaulthost, $plusdomain, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_DEFAULTDOMAIN"),
						 pql_get_define("PQL_GLOB_ATTR_DEFAULTHOST"),
						 pql_get_define("PQL_GLOB_ATTR_PLUSDOMAIN"));
		foreach($attribs as $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
													$attrib, $$attrib);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('default host / domain options')))
			. ": " . pql_format_error(0);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('default host / domain options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function defaultdomain_help() {
	global $LANG;
?>
	<b>Default domain name</b>.<br>
	Default:  me, if that is supplied, otherwise the literal name defaultdomain,
	which is probably not what you want.  qmail-inject adds
	this name to any host name without dots, including
	defaulthost if defaulthost does not have dots.<br>
	(Exception: see plusdomain.)
	<br><br>
	The QMAILDEFAULTDOMAIN environment variable overrides
	defaultdomain.
	<br><br>
	<b>Default host name</b>.  <br>
	Default:  me, if that is supplied, otherwise the literal name defaulthost, which is
	probably not what you want.  qmail-inject adds this
	name to any address without a host name.  defaulthost
	need not be the current host's name.  For example, you
	may prefer that outgoing mail show just your domain
	name.
	<br><br>
	The QMAILDEFAULTHOST environment variable overrides
	defaulthost.
	<br><br>
	<b>Plus domain name.</b>
	<br>Default:  me, if that is supplied;
	otherwise the literal name plusdomain, which is
	probably not what you want.  qmail-inject adds this
	name to any host name that ends with a plus sign,
	including defaulthost if defaulthost ends with a plus
	sign.  If a host name does not have dots but ends with
	a plus sign, qmail-inject uses plusdomain, not
	defaultdomain.
	<br><br>
	The QMAILPLUSDOMAIN environment variable overrides
	plusdomain.
<?php
}

function defaultdomain_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
