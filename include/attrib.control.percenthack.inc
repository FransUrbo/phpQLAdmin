<?php
// control attribute plugin for
// percenthack (allowed domains for %-relaying)
//
// $Id: attrib.control.percenthack.inc,v 2.25 2007-02-15 12:07:57 turbo Exp $

// {{{ percenthack_check(type)
function percenthack_check($type) {
	global $error, $LANG;
	
	// check each field
	if(@is_array($_REQUEST["percenthack"])) {
		foreach($_REQUEST["percenthack"] as $key => $server) {
			if(empty($server))
			  continue;
			
			// it is a valid domain name
			if(!pql_check_hostaddress($server)) {
				$is_error = true;
				$error["percenthack_$key"] = $LANG->_('Invalid');
			}
			
		} // end foreach
	} // end if is_array...

	if($is_error)
	  return false;
	
	return true;
}
// }}}

// {{{ percenthack_init(host)
// init values
function percenthack_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["percenthack"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_PERCENTHACK"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["percenthack"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_PERCENTHACK"));
	
	if(!@$_REQUEST["percenthack"])
	  return true;
	elseif(!@is_array($_REQUEST["percenthack"]))
	  $_REQUEST["percenthack"] = array($_REQUEST["percenthack"]);
}
// }}}


// {{{ percenthack_print_view(host)
function percenthack_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	percenthack_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">percenthack
      <tr>
        <td class="title" colspan="3"><?php echo $LANG->_('Percent relaying domains', 4)?>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_PERCENTHACK");

	$tbl_txt = array('tit' => $LANG->_('Domain'), 'add' => $LANG->_('Add domain'), 'del' => $LANG->_('Remove domain'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["percenthack"],
							pql_get_define("PQL_ATTR_PERCENTHACK"), $class);
?>
    </th>
  </table>
<?php
}
// }}}

// {{{ percenthack_print_form(void)
function percenthack_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["percenthack"]); $i++) { ?>
    <input type="hidden" name="percenthack[<?php echo $i?>]" value="<?=$_REQUEST["percenthack"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">percenthack (<?php echo $LANG->_('Percent relaying domains', 4)?>)</th>
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?php echo $LANG->_('Domain')?></td>
          <td class="<?php echo $class?>">
            <?php echo pql_format_error_span($error["percenthack"][$i]); ?>
            <input type="text" name="percenthack[<?php echo $i?>]" size="40">
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

// {{{ percenthack_save(type, host)
function percenthack_save($type, $host) {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_PERCENTHACK"), '', $_REQUEST["percenthack"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_PERCENTHACK"), '', $_REQUEST["percenthack"])) 
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('percenthack options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('percenthack options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		if(pql_modify_attribute($host,
								pql_get_define("PQL_ATTR_PERCENTHACK"), $_REQUEST["percenthack"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('Percent hack options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Percent hack options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ percenthack_help(void)
function percenthack_help() {
	global $LANG;
?>
	List of domain names where the percent hack is applied.
	If domain is listed in percenthack, any address of the
	form user%fqdn@domain is rewritten as user@fqdn.  user
	may contain %, so the percent hack may be applied
	repeatedly.  qmail-send handles percenthack before
	locals.
<?php
}
// }}}

// {{{ percenthack_help_cr(void)
function percenthack_help_cr() {
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
