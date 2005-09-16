<?php
// control attribute plugin for
// percenthack (allowed domains for %-relaying)
//
// $Id: attrib.control.percenthack.inc,v 2.21 2005-09-16 06:08:49 turbo Exp $

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
	$_REQUEST["percenthack"] = pql_get_attribute($_pql_control->ldap_linkid,
												 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												 pql_get_define("PQL_ATTR_PERCENTHACK"));
	
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
        <td class="title" colspan="3"><?=$LANG->_('Percent relaying domains', 4)?>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_PERCENTHACK");

	$tbl_txt = array('tit' => $LANG->_('Domain'), 'add' => $LANG->_('Add domain'), 'del' => $LANG->_('Remove domain'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single($_pql_control->ldap_linkid, $tbl_txt, $tbl_url, $_REQUEST["percenthack"],
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
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["percenthack"]); $i++) { ?>
    <input type="hidden" name="percenthack[<?=$i?>]" value="<?=$_REQUEST["percenthack"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">percenthack (<?=$LANG->_('Percent relaying domains', 4)?>)</th>
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?=$LANG->_('Domain')?></td>
          <td class="<?=$class?>">
            <?php echo pql_format_error_span($error["percenthack"][$i]); ?>
            <input type="text" name="percenthack[<?=$i?>]" size="40">
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

// {{{ percenthack_save(type, host)
function percenthack_save($type, $host) {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PERCENTHACK"), '', $_REQUEST["percenthack"])) 
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('percenthack options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('percenthack options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;


	  case "del":
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
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
