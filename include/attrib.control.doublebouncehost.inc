<?php
// control attribute plugin for
// doublebouncehost (username of bounce sender)
// doublebounceto (hostname of bounce sender)
//
// $Id: attrib.control.doublebouncehost.inc,v 2.20 2005-09-16 06:08:46 turbo Exp $

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
	foreach($attribs as $key => $attrib) {
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid,
											pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											$attrib);
	}
}
// }}}


// {{{ doublebouncehost_print_view(host)
function doublebouncehost_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	doublebouncehost_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">doublebouncehost, doublebounceto (Double bounce)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Receiver')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["doublebounceto"]) { echo $_REQUEST["doublebounceto"]; } else { echo $LANG->_('Default').": postmaster"; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_DOUBLEBOUNCETO")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DOUBLEBOUNCETO")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Host')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["doublebouncehost"]) { echo pql_maybe_idna_decode($_REQUEST["doublebouncehost"]); } else { echo $LANG->_('Default').': '.pql_maybe_idna_decode($host)." (".$LANG->_('Taken from \'me\'').")"; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST")?>"></a></td>
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
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Double bounce</th>
<?php if($_REQUEST["attrib"] == "doublebouncehost") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["doublebouncehost"]); ?><input type="text" name="doublebouncehost" value="<?=$_REQUEST["doublebouncehost"]?>"> <?=$LANG->_('Default')?>: <?=$_REQUEST["mxhost"]?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "doublebounceto") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Receiver')?></td>
          <td><?php echo pql_format_error_span($error["doublebounceto"]); ?><input type="text" name="doublebounceto" value="<?=$_REQUEST["doublebounceto"]?>"> <?=$LANG->_('Default')?>: postmaster</td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.
          </td>
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

// {{{ doublebouncehost_save(type, host)
function doublebouncehost_save($type, $host) {
	global $_pql_control, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		$attribs = array("doublebouncehost" => pql_get_define("PQL_ATTR_DOUBLEBOUNCEHOST"),
						 "doublebounceto"   => pql_get_define("PQL_ATTR_DOUBLEBOUNCETO"));
		foreach($attribs as $key => $attrib) {
		  if(!@empty($_REQUEST[$key]))
			$return[] = pql_modify_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											 $attrib, 1, $_REQUEST[$key]);
		}

		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('double bouncing options')))
			. ": " . pql_format_error(0);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('double bouncing options')));
		
		attribute_forward($msg);
		break;

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
	<b><?=$LANG->_('Host')?> (doublebouncehost)</b><br>
	Double-bounce host. Default: <b><?=$_REQUEST["mxhost"]?></b> (taken from 'me').
	If that is supplied; otherwise the literal name doublebouncehost, which is
	probably not what you want.Host name of double bounce sender.
	<br><br>
<?php } if($_REQUEST["attrib"] == "doublebounceto") { ?>
	<b><?=$LANG->_('Receiver')?> (doublebounceto)</b>
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
