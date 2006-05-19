<?php
// control attribute plugin for
// bouncefrom (username of bounce sender)
// bouncehost (hostname of bounce sender)
//
// $Id: attrib.control.bouncefrom.inc,v 2.21 2006-05-19 06:55:31 turbo Exp $

// {{{ bouncefrom_check(type)

function bouncefrom_check($type) {
	global $error, $LANG;
	$attrib = $_REQUEST["attrib"];

	// check validity
	if($attrib == "bouncefrom") {
	  if(($_REQUEST["bouncefrom"] != "") and !pql_check_email($_REQUEST["bouncefrom"] . "@domain.tld")) {
		$error["bouncefrom"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	}

	// check validity
	if($attrib == "bouncehost") {
	  if(($_REQUEST["bouncehost"] != "") and !pql_check_hostaddress($_REQUEST["bouncehost"])) {
		$error["bouncehost"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	}
	
	if($is_error == false)
	  return true;
}

// }}}

// {{{ bouncefrom_init(host)

// init values
function bouncefrom_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$attribs = array("bouncefrom" => pql_get_define("PQL_ATTR_BOUNCEFROM"),
					 "bouncehost" => pql_get_define("PQL_ATTR_BOUNCEHOST"));
	if($host == "Global") {
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
											$attrib, 0, pql_get_define("PQL_ATTR_CN")."=*");
	} else {
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid,
											pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											$attrib);
	}
}

// }}}


// {{{ bouncefrom_print_view(host)
function bouncefrom_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	bouncefrom_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">bouncefrom, bouncehost (<?=$LANG->_('Bouncing source')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('User for bouncing')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["bouncefrom"]) { echo $_REQUEST["bouncefrom"]; } else { echo $LANG->_('Default').": MAILER-DAEMON"; }?></td>
        <td class="<?=$class?>"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_BOUNCEFROM")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_BOUNCEFROM")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Host for bouncing')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["bouncehost"]) { echo pql_maybe_idna_decode($_REQUEST["bouncehost"]); } else { echo $LANG->_('Default') . ': ' . pql_maybe_idna_decode($host)." (" . $LANG->_('Taken from \'me\'') . ")";}?></td>
        <td class="<?=$class?>"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_BOUNCEHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_BOUNCEHOST")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ bouncefrom_print_form(void)
function bouncefrom_print_form() {
	global $LANG, $error;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">bouncefrom, bouncehost (<?=$LANG->_('Bouncing source')?>)</th>
<?php if($_REQUEST["attrib"] == 'bouncefrom') { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('User for bouncing')?></td>
          <td><?php echo pql_format_error_span($error["bouncefrom"]); ?><input type="text" name="bouncefrom" value="<?=$_REQUEST["bouncefrom"]?>"> <?=$LANG->_('Default')?>: MAILER-DAEMON</td>
        </tr>

<?php } if($_REQUEST["attrib"] == 'bouncehost') { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host for bouncing')?></td>
          <td><?php echo pql_format_error_span($error["bouncehost"]); ?><input type="text" name="bouncehost" value="<?=$_REQUEST["bouncehost"]?>"> <?=$LANG->_('Default')?>: <?=$_REQUEST["mxhost"]?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('The combination of user and host will return the email address used for bouning messages, e.g. MAILER-DAEMON@mail.host.ch')?>.<br><?=$LANG->_('Leave the fields empty to use default configuration')?>.
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

// {{{ bouncefrom_save(type, host)
function bouncefrom_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		$attribs = array("bouncefrom" => pql_get_define("PQL_ATTR_BOUNCEFROM"),
						 "bouncehost" => pql_get_define("PQL_ATTR_BOUNCEHOST"));
		foreach($attribs as $key => $attrib) {
		  if(@$_REQUEST[$key]) {
			if($host == "Global") {
			  // Get all QLC host
			  $qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
								 '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
			  
			  $success = 1;
			  foreach($qlcs as $host) {
				if(!pql_modify_attribute($_pql_control->ldap_linkid, $host, $attrib, '', $_REQUEST[$key]))
				  $success = 0;
			  }
			} else {
			  if(pql_modify_attribute($_pql_control->ldap_linkid,
									  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
									  $attrib, 1, $_REQUEST[$key]))
				$success = 1;
			}
		  }
		}
		
		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('bouncing options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bouncing options')))
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


// {{{ bouncefrom_help(void)
function bouncefrom_help() {
	global $LANG;
?>
<?php if($_REQUEST["attrib"] == 'bouncefrom') { ?>
	<b>User (bouncefrom)</b>
	<br>
	Bounce username.  Default:  MAILER-DAEMON.
	<br><br>
<?php } if($_REQUEST["attrib"] == 'bouncehost') { ?>
 	<b>Host (bouncehost)</b>
	<br>
	Bounce host. Default: <b><?=$_REQUEST["mxhost"]?></b> (taken from 'me').
    If that is supplied; otherwise the literal name bouncehost, which is
	probably not what you want.  If a message is
	permanently undeliverable, qmail-send sends a single-
	bounce notice back to the message's envelope sender.
	The notice is From: bouncefrom@bouncehost, although its
	envelope sender is empty.
<?php } 
}
// }}}

// {{{ bouncefrom_help_cr(void)
function bouncefrom_help_cr() {
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
