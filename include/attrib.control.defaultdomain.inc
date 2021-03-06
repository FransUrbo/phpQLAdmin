<?php
// control attribute plugin for
// defaultdomain (default domain)
// defaulthost (default hostname)
//
// $Id: attrib.control.defaultdomain.inc,v 2.23 2007-02-15 12:07:19 turbo Exp $

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
	if($host == "Global") {
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
											$attrib, 0, pql_get_define("PQL_ATTR_CN")."=*");
	} else {
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = $_pql_control->get_attribute($host, $attrib);
	}

}
// }}}


// {{{ defaultdomain_print_view(host)
function defaultdomain_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	defaultdomain_init($host);
	$cn = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">defaultdomain, defaulthost, plusdomain (<?php echo $LANG->_('Default domain / host')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Default domain')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["defaultdomain"]) { echo $_REQUEST["defaultdomain"]; } else { pql_maybe_idna_decode($cn) . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";}?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_DEFAULTDOMAIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DEFAULTDOMAIN")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Default host')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["defaulthost"]) { echo $_REQUEST["defaulthost"]; } else { pql_maybe_idna_decode($cn) . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_DEFAULTHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DEFAULTHOST")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Plus domain')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["plusdomain"]) { echo $_REQUEST["plusdomain"]; } else { pql_maybe_idna_decode($cn) . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_PLUSDOMAIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_PLUSDOMAIN")?>"></a></td>
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
<?php if($_REQUEST["attrib"] == "defaultdomain") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Default domain')?></td>
          <td><?php echo pql_format_error_span($error["defaultdomain"]); ?><input type="text" name="defaultdomain" value="<?php echo $_REQUEST["defaultdomain"]?>"> <?php echo $LANG->_('Default')?>: <?php echo $_REQUEST["mxhost"]?> (<?php echo $LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "defaulthost") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Default host')?></td>
          <td><?php echo pql_format_error_span($error["defaulthost"]); ?><input type="text" name="defaulthost" value="<?php echo $_REQUEST["defaulthost"]?>"> <?php echo $LANG->_('Default')?>: <?php echo $_REQUEST["mxhost"]?> (<?php echo $LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "plusdomain") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Plus domain')?></td>
          <td><?php echo pql_format_error_span($error["plusdomain"]); ?><input type="text" name="plusdomain" value="<?php echo $_REQUEST["plusdomain"]?>"> <?php echo $LANG->_('Default')?>: <?php echo $_REQUEST["mxhost"]?> (<?php echo $LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ defaultdomain_save(type, host)
function defaultdomain_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		$attribs = array("defaultdomain" => pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
						 "defaulthost"   => pql_get_define("PQL_ATTR_DEFAULTHOST"),
						 "plusdomain"    => pql_get_define("PQL_ATTR_PLUSDOMAIN"));
		foreach($attribs as $key => $attrib) {
		  if(!@empty($_REQUEST[$key])) {
			if($host == "Global") {
			  // Get all QLC host
			  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
								 '(&(cn=*)(objectclass=qmailControl))');
			  
			  $success = 1;
			  foreach($qlcs as $host) {
				if(!pql_modify_attribute($host, $attrib, '', $_REQUEST[$key]))
				  $success = 0;
			  }
			} else {
			  if(pql_modify_attribute($host,
									  $attrib, 1, $_REQUEST[$key]))
				$success = 1;
			}
		  }
		}
		
		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('default host / domain options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('default host / domain options')))
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
