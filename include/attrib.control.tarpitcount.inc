<?php
// control attribute plugin for
// tarpitcount (user id of virtual user)
// tarpitdelay (group id of virtual user)
//
// $Id: attrib.control.tarpitcount.inc,v 2.24 2007-02-15 12:08:05 turbo Exp $

// {{{ tarpitcount_check(type)
function tarpitcount_check($type) {
	global $error, $LANG;

	// tarPitCount
	if($_REQUEST["attrib"] == "tarpitcount") {
	  if(@empty($_REQUEST["tarpitcount"])) {
		$error["tarpitcount"] = $LANG->_('Missing');
		$is_error = true;
	  }
	  
	  // check validity
	  if(preg_match("/[^0-9]/", $_REQUEST["tarpitcount"])) {
		$error["tarpitcount"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	}

	// tarPitDelay
	if($_REQUEST["attrib"] == "tarpitdelay") {
	  if(@empty($_REQUEST["tarpitdelay"])) {
		$error["tarpitdelay"] = $LANG->_('Missing');
		$is_error = true;
	  }
	  
	  // check validity
	  if(preg_match("/[^0-9]/", $_REQUEST["tarpitdelay"])) {
		$error["tarpitdelay"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	  
	  // must be > 0
	  if(($error["tarpitdelay"] == "") and ($_REQUEST["tarpitdelay"] <= 0)) {
		$error["tarpitdelay"] = $LANG->_('Must be above zero');
		$is_error = true;
	  }
	}
	
	if($is_error == false)
	  return true;
}
// }}}

// {{{ tarpitcount_init(host)
// init values
function tarpitcount_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$attribs = array("tarpitcount" => pql_get_define("PQL_ATTR_TARPITCOUNT"),
					 "tarpitdelay" => pql_get_define("PQL_ATTR_TARPITDELAY"));
	if($host == "Global")
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
											$attrib, 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = $_pql_control->get_attribute($host, $attrib);
}
// }}}


// {{{ tarpitcount_print_view(host)
function tarpitcount_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	tarpitcount_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">tarpitcount, tarpitdelay (<?php echo $LANG->_('Tarpitting settings')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Tarpitting settings')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["tarpitcount"]) { echo pql_complete_constant($LANG->_('Delay submitting after %value% RCPTO\'s'), array('value' => $_REQUEST["tarpitcount"])); } else { echo '<i>'.$LANG->_('Disabled').'</i>'; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_TARPITCOUNT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options', 4).' '.pql_get_define("PQL_ATTR_TARPITCOUNT")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Tarpit delay')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["tarpitdelay"]) { echo $_REQUEST["tarpitdelay"]; } else { echo $LANG->_('Default').': 5'; }?> <?php echo $LANG->_('Seconds', 4)?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_TARPITDELAY")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options', 4).' '.pql_get_define("PQL_ATTR_TARPITDELAY")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ tarpitcount_print_form(void)
function tarpitcount_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">tarpitcount, tarpitdelay (<?php echo $LANG->_('Tarpitting settings')?>)</th>
<?php if($_REQUEST["attrib"] == "tarpitcount") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Tarpit count')?></td>
          <td><?php echo pql_format_error_span($error["tarpitcount"]); ?><input type="text" name="tarpitcount" value="<?php echo $_REQUEST["tarpitcount"]?>"> RCPTOs</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "tarpitdelay") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Tarpit delay')?></td>
          <td><?php echo pql_format_error_span($error["tarpitdelay"]); ?><input type="text" name="tarpitdelay" value="<?php echo $_REQUEST["tarpitdelay"]?>"> <?php echo $LANG->_('Seconds', 4)?></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_('Set tarpit count to 0 to disable tarpitting')?></td>
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

// {{{ tarpitcount_save(type, host)
function tarpitcount_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		$attribs = array("tarpitcount" => pql_get_define("PQL_ATTR_TARPITCOUNT"),
						 "tarpitdelay" => pql_get_define("PQL_ATTR_TARPITDELAY"));
		foreach($attribs as $key => $attrib) {
		  // Allow 'tarpit{Count,Delay}' to be '0' (disable).
		  if(@isset($_REQUEST[$key])) {
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
									   array('what' => $LANG->_('tarpitting options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('tarpitting options'))) . ": " . pql_format_error(0);
																							 
		attribute_forward($msg);
		break;
		// }}}

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ tarpitcount_help(void)
function tarpitcount_help() {
    global $LANG;
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
?>
<?php if($_REQUEST["attrib"] == "tarpitcount") { ?>
	<b><?php echo $LANG->_('Tarpit count')?></b>
	<br>
	The number of RCPT TOs you accept before you start tarpitting. Tarpitting means introducing an delay after each subsequent RCPT TO, therefore sending messages to lots of recipient takes a _lot_ of time and makes life for spammers harder.
	Default: 0 (disabled)
	<br><br>
<?php } if($_REQUEST["attrib"] == "tarpitdelay") { ?>
	<b><?php echo $LANG->_('Tarpit delay')?></b>
	<br>
	The number of seconds of delay for tarpitting. Default: 5
<?php }
}
// }}}

// {{{ tarpitcount_help_cr(void)
function tarpitcount_help_cr() {
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
