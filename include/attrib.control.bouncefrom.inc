<?php
// control attribute plugin for
// bouncefrom (username of bounce sender)
// bouncehost (hostname of bounce sender)
//
// $Id: attrib.control.bouncefrom.inc,v 2.11 2003-11-20 08:01:36 turbo Exp $

function bouncefrom_check($type) {
	global $error, $bouncefrom, $bouncehost, $view, $LANG;

	// check validity
	if($bouncefrom != "" and !pql_check_email($bouncefrom . "@domain.tld")) {
		$error["bouncefrom"] = $LANG->_('Invalid');
		$is_error = true;
	}

	// check validity
	if($bouncehost != "" and !pql_check_hostaddress($bouncehost)) {
		$error["bouncehost"] = $LANG->_('Invalid');
		$is_error = true;
	}
	
	if($is_error == false)
	  return true;
}

// init values
function bouncefrom_init($host) {
	global $_pql_control, $bouncefrom, $bouncehost, $view;

	// fetch data from ldap server
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_BOUNCEFROM"),
					 pql_get_define("PQL_GLOB_ATTR_BOUNCEHOST"));

	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);

		if(!is_null($value))
		  $$attrib = $value[0];
	}
}

function bouncefrom_print_view($host) {
	global $_pql_control, $bouncefrom, $bouncehost, $view, $LANG;
	
	// init data
	bouncefrom_init($host);
	
	// set empty fields to their default value
	if($bouncefrom == "")
	  $bouncefrom = $LANG->_('Default').": MAILER-DAEMON";

	if($bouncehost == "")
	  $bouncehost = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">bouncefrom, bouncehost (<?=$LANG->_('Bouncing source')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('User for bouncing')?></td>
        <td><?=$bouncefrom?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Host for bouncing')?></td>
        <td><?=$bouncehost?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=bouncefrom"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function bouncefrom_print_form() {
	global $attrib, $bouncefrom, $bouncehost, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">bouncefrom, bouncehost (<?=$LANG->_('Bouncing source')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('User for bouncing')?></td>
          <td><?php echo pql_format_error_span($error["bouncefrom"]); ?><input type="text" name="bouncefrom" value="<?=$bouncefrom?>"> <?=$LANG->_('Default')?>: MAILER-DAEMON</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host for bouncing')?></td>
          <td><?php echo pql_format_error_span($error["bouncehost"]); ?><input type="text" name="bouncehost" value="<?=$bouncehost?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('The combination of user and host will return the email address used for bouning messages, e.g. MAILER-DAEMON@mail.host.ch')?>.<br><?=$LANG->_('Leave the fields empty to use default configuration')?>.
          </td>
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

function bouncefrom_save($type, $host) {
	global $_pql_control, $attrib, $bouncefrom, $bouncehost, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_BOUNCEFROM"),
						 pql_get_define("PQL_GLOB_ATTR_BOUNCEHOST"));
		foreach($attribs as $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
													$attrib, $$attrib);
		
		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('bouncing options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('bouncing options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function bouncefrom_help() {
	global $LANG;
?>
	<b>User (bouncefrom)</b>
	<br>
	Bounce username.  Default:  MAILER-DAEMON.
	<br><br>
 	<b>Host (bouncehost)</b>
	<br>
	Bounce host.  Default:  me, if that is supplied;
	otherwise the literal name bouncehost, which is
	probably not what you want.  If a message is
	permanently undeliverable, qmail-send sends a single-
	bounce notice back to the message's envelope sender.
	The notice is From: bouncefrom@bouncehost, although its
	envelope sender is empty.
<?php
}

function bouncefrom_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
