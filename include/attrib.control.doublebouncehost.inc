<?php
// control attribute plugin for
// doublebouncehost (username of bounce sender)
// doublebounceto (hostname of bounce sender)
//
// $Id: attrib.control.doublebouncehost.inc,v 2.24 2007-02-15 12:07:20 turbo Exp $

// {{{ doublebouncehost_check(type)
function doublebouncehost_check($type) {
	global $error, $LANG;

	// check validity
	if($_REQUEST["attrib"] == "doublebounceto") {
	  if(($_REQUEST["doublebounceto"] != "") and !pql_check_email($_REQUEST["doublebounceto"] . "@domain.tld")) {
		$error["doublebounceto"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	}

	// check validity
	if($_REQUEST["attrib"] == "doublebouncehost") {
	  if(($_REQUEST["doublebouncehost"] != "") and !pql_check_hostaddress($_REQUEST["doublebouncehost"])) {
		$error["doublebouncehost"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	}

	if($is_error == false)
	  return true;
}
// }}}

// {{{ doublebouncehost_init(host)
// init values
function doublebouncehost_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$attribs = array("doublebouncehost" => pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST"),
					 "doublebounceto"   => pql_get_define("PQL_ATTR_DOUBLEBOUNCETO"));
	if($host == "Global")
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
											$attrib, 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = $_pql_control->get_attribute($host, $attrib);
}
// }}}


// {{{ doublebouncehost_print_view(host)
function doublebouncehost_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	doublebouncehost_init($host);
	$cn = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">doublebouncehost, doublebounceto (Double bounce)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Receiver')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["doublebounceto"]) { echo $_REQUEST["doublebounceto"]; } else { echo $LANG->_('Default').": postmaster"; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_DOUBLEBOUNCETO")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DOUBLEBOUNCETO")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Host')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["doublebouncehost"]) { echo pql_maybe_idna_decode($_REQUEST["doublebouncehost"]); } else { echo $LANG->_('Default').': '.pql_maybe_idna_decode($cn)." (".$LANG->_('Taken from \'me\'').")"; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ doublebouncehost_print_form(void)
function doublebouncehost_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Double bounce</th>
<?php if($_REQUEST["attrib"] == "doublebouncehost") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["doublebouncehost"]); ?><input type="text" name="doublebouncehost" value="<?php echo $_REQUEST["doublebouncehost"]?>"> <?php echo $LANG->_('Default')?>: <?php echo $_REQUEST["mxhost"]?> (<?php echo $LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "doublebounceto") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Receiver')?></td>
          <td><?php echo pql_format_error_span($error["doublebounceto"]); ?><input type="text" name="doublebounceto" value="<?php echo $_REQUEST["doublebounceto"]?>"> <?php echo $LANG->_('Default')?>: postmaster</td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_('Leave the fields empty to use default configuration')?>.
          </td>
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

// {{{ doublebouncehost_save(type, host)
function doublebouncehost_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		$attribs = array("doublebouncehost" => pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST"),
						 "doublebounceto"   => pql_get_define("PQL_ATTR_DOUBLEBOUNCETO"));
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
									   array('what' => $LANG->_('double bouncing options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('double bouncing options')))
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

// {{{ doublebouncehost_help(void)
function doublebouncehost_help() {
	global $LANG;
?>
<?php if($_REQUEST["attrib"] == "doublebouncehost") { ?>
	<b><?php echo $LANG->_('Host')?> (doublebouncehost)</b><br>
	Double-bounce host. Default: <b><?php echo $_REQUEST["mxhost"]?></b> (taken from 'me').
	If that is supplied; otherwise the literal name doublebouncehost, which is
	probably not what you want.Host name of double bounce sender.
	<br><br>
<?php } if($_REQUEST["attrib"] == "doublebounceto") { ?>
	<b><?php echo $LANG->_('Receiver')?> (doublebounceto)</b>
	<br>
	User to receive double-bounces.  Default:  postmaster.
	If a single-bounce notice is permanently undeliverable,
	qmail-send sends a double-bounce notice to
	doublebounceto@doublebouncehost.  (If that bounces,
	qmail-send gives up.)
<?php }
}
// }}}

// {{{ doublebouncehost_help_cr(void)
function doublebouncehost_help_cr() {
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
