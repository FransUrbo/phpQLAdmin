<?php
// control attribute plugin for
// defaultdomain (default domain)
// defaulthost (default hostname)
//
// $Id: attrib.control.defaultdomain.inc,v 2.19.2.1 2006-03-23 09:13:15 turbo Exp $

// {{{ defaultdomain_check(type)
function defaultdomain_check($type) {
	global $error, $LANG;

	// check validity
	if(($_REQUEST["defaultdomain"] != "") and !pql_check_hostaddress($_REQUEST["defaultdomain"])) {
		$error["defaultdomain"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if(($_REQUEST["defaulthost"] != "") and !pql_check_hostaddress($_REQUEST["defaulthost"])) {
		$error["defaulthost"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if(($_REQUEST["plusdomain"] != "") and !pql_check_hostaddress($_REQUEST["plusdomain"])) {
		$error["plusdomain"] = $LANG->_('Invalid');
		$is_error = true;
	}

	if($is_error == false)
	  return true;
}
// }}}

// {{{ defaultdomain_init(host)
// init values
function defaultdomain_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$attribs = array("defaultdomain" => pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
					 "defaulthost"   => pql_get_define("PQL_ATTR_DEFAULTHOST"),
					 "plusdomain"    => pql_get_define("PQL_ATTR_PLUSDOMAIN"));
	foreach($attribs as $key => $attrib) {
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid,
											pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											$attrib);
	}
}
// }}}


// {{{ defaultdomain_print_view(host)
function defaultdomain_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	defaultdomain_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">defaultdomain, defaulthost, plusdomain (<?=$LANG->_('Default domain / host')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Default domain')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["defaultdomain"]) { echo $_REQUEST["defaultdomain"]; } else { pql_maybe_idna_decode($host) . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";}?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_DEFAULTDOMAIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DEFAULTDOMAIN")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Default host')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["defaulthost"]) { echo $_REQUEST["defaulthost"]; } else { pql_maybe_idna_decode($host) . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_DEFAULTHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DEFAULTHOST")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Plus domain')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["plusdomain"]) { echo $_REQUEST["plusdomain"]; } else { pql_maybe_idna_decode($host) . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_PLUSDOMAIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_PLUSDOMAIN")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ defaultdomain_print_form(void)
function defaultdomain_print_form() {
	global $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
<?php if($_REQUEST["attrib"] == "defaultdomain") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default domain')?></td>
          <td><?php echo pql_format_error_span($error["defaultdomain"]); ?><input type="text" name="defaultdomain" value="<?=$_REQUEST["defaultdomain"]?>"> <?=$LANG->_('Default')?>: <?=$_REQUEST["mxhost"]?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "defaulthost") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default host')?></td>
          <td><?php echo pql_format_error_span($error["defaulthost"]); ?><input type="text" name="defaulthost" value="<?=$_REQUEST["defaulthost"]?>"> <?=$LANG->_('Default')?>: <?=$_REQUEST["mxhost"]?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "plusdomain") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Plus domain')?></td>
          <td><?php echo pql_format_error_span($error["plusdomain"]); ?><input type="text" name="plusdomain" value="<?=$_REQUEST["plusdomain"]?>"> <?=$LANG->_('Default')?>: <?=$_REQUEST["mxhost"]?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ defaultdomain_save(type, host)
function defaultdomain_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		$attribs = array("defaultdomain" => pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
						 "defaulthost"   => pql_get_define("PQL_ATTR_DEFAULTHOST"),
						 "plusdomain"    => pql_get_define("PQL_ATTR_PLUSDOMAIN"));
		foreach($attribs as $key => $attrib) {
		  if(!@empty($_REQUEST[$key]))
			$return[] = pql_modify_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											 $attrib, 1, $_REQUEST[$key]);
		}
		
		if(in_array(false, $return)) {
		  pql_format_error();
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('default host / domain options')));
		} else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('default host / domain options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ defaultdomain_help(void)
function defaultdomain_help() {
	global $LANG;
?>
<?php if($_REQUEST["attrib"] == "defaultdomain") { ?>
	<b>Default domain name</b>.<br>
	Default:  me, if that is supplied, otherwise the literal name defaultdomain,
	which is probably not what you want.  qmail-inject adds
	this name to any host name without dots, including
	defaulthost if defaulthost does not have dots.<br>
	(Exception: see plusdomain.)
	<br><br>
	The QMAILDEFAULTDOMAIN environment variable overrides
	defaultdomain.
<?php } if($_REQUEST["attrib"] == "defaulthost") { ?>
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
<?php } if($_REQUEST["attrib"] == "plusdomain") { ?>
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
<?php }
}
// }}}

// {{{ defaultdomain_help_cr(void)
function defaultdomain_help_cr() {
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
