<?php
// control attribute plugin for
// concurrencylocal (may simultaneous local deliveries)
// concurrencyremote (may simultaneous remote deliveries)
//
// $Id: attrib.control.concurrencylocal.inc,v 2.26 2007-02-15 12:07:19 turbo Exp $

// {{{ concurrencylocal_check(type)
function concurrencylocal_check($type) {
	global $error, $LANG;

	// concurrencyLocal
	if($_REQUEST["attrib"] == "concurrencylocal") {
	  if(preg_match("/[^0-9]/", $_REQUEST["concurrencylocal"])) {
		$error["concurrencylocal"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	}

	// concurrencyRemote
	if($_REQUEST["attrib"] == "concurrencyremote") {
	  if(preg_match("/[^0-9]/", $_REQUEST["concurrencyremote"])) {
		$error["concurrencyremote"] = $LANG->_('Invalid');
		$is_error = true;
	  }
	}

	if($is_error == false)
	  return true;
}
// }}}

// {{{ concurrencylocal_init(host)
// init values
function concurrencylocal_init($host) {
	global $_pql_control, $concurrencylocal, $concurrencyremote, $view;

	// fetch data from ldap server
	$attribs = array("concurrencylocal"  => pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
					 "concurrencyremote" => pql_get_define("PQL_ATTR_CONCURRENCYREMOTE"));
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


// {{{ concurrencylocal_print_view(host)
function concurrencylocal_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	concurrencylocal_init($host);

	// set empty fields to their default value
	if($_REQUEST["concurrencylocal"] == "")	  $LANG->_('Default', 4).': '.$_REQUEST["concurrencylocal"] = "10";
	if($_REQUEST["concurrencylocal"] == 0)	  $_REQUEST["concurrencylocal"] = "0 (".$LANG->_('local delivery disabled').")";
	
	if($_REQUEST["concurrencyremote"] == "")  $LANG->_('Default', 4).': '.$_REQUEST["concurrencyremote"] = "20";
	if($_REQUEST["concurrencyremote"] == 0)	  $_REQUEST["concurrencyremote"] = "0 (".$LANG->_('remote delivery disabled').")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">concurrencylocal, concurrencyremote<br>(<?php echo $LANG->_('Maximum simultaneous deliveries')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Local')?></td>
        <td class="<?php echo $class?>"><?=$_REQUEST["concurrencylocal"]?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_CONCURRENCYLOCAL")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_CONCURRENCYLOCAL")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Remote')?></td>
        <td class="<?php echo $class?>"><?=$_REQUEST["concurrencyremote"]?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_CONCURRENCYREMOTE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_CONCURRENCYREMOTE")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ concurrencylocal_print_form(void)
function concurrencylocal_print_form() {
	global $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">concurrencylocal, concurrencyremote (<?php echo $LANG->_('Maximum simultaneous deliveries')?>)</th>
<?php if($_REQUEST["attrib"] == "concurrencylocal") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Local')?></td>
          <td><?php echo pql_format_error_span($error["concurrencylocal"]); ?><input type="text" name="concurrencylocal" value="<?php echo $_REQUEST["concurrencylocal"]?>"> <?=$LANG->_('Default')?>: 10</td>
        </tr>

<?php } if($_REQUEST["attrib"] == "concurrencyremote") { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Remote')?></td>
          <td><?php echo pql_format_error_span($error["concurrencyremote"]); ?><input type="text" name="concurrencyremote" value="<?php echo $_REQUEST["concurrencyremote"]?>"> <?=$LANG->_('Default')?>: 20</td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_('Leave the fields empty to set the default values')?>.</td>
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

// {{{ concurrencylocal_save(type, host)
function concurrencylocal_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		$attribs = array("concurrencylocal"  => pql_get_define("PQL_ATTR_CONCURRENCYLOCAL"),
						 "concurrencyremote" => pql_get_define("PQL_ATTR_CONCURRENCYREMOTE"));
		foreach($attribs as $key => $attrib) {
		  // Allow for value to be '0'.
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


// {{{ concurrencylocal_help(void)
function concurrencylocal_help() {
	global $LANG;
?>
<?php if($_REQUEST["attrib"] == "concurrencylocal") { ?>
	<b><?php echo $LANG->_('Local')?></b><br>
	Maximum number of simultaneous local delivery attempts.
	Default: 10.  If 0, local deliveries will be put on
	hold.  concurrencylocal is limited at compile time to
	120.
<?php } if($_REQUEST["attrib"] == "concurrencyremote") { ?>
	<b><?php echo $LANG->_('Remote')?></b><br>
	Maximum number of simultaneous remote delivery
	attempts.  Default: 20.  If 0, remote deliveries will
	be put on hold.  concurrencyremote is limited at
	compile time to 120.
<?php }
}
// }}}

// {{{ concurrencylocal_help_cr(void)
function concurrencylocal_help_cr() {
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
