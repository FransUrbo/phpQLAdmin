<?php
// control attribute plugin for
// percenthack (allowed domains for %-relaying)
//
// $Id: attrib.control.percenthack.inc,v 2.18 2004-10-19 10:40:41 turbo Exp $

// {{{ percenthack_check(type)
function percenthack_check($type) {
	global $error, $LANG;
	
	// check each field
	if(is_array($_REQUEST["percenthack"])) {
		foreach($_REQUEST["percenthack"] as $key => $server) {
			if($server == "")
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
	
	if(is_null($_REQUEST["percenthack"]))
	  return true;
}
// }}}


// {{{ percenthack_print_view(host)
function percenthack_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	percenthack_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">percenthack (<?=$LANG->_('Percent relaying domains', 4)?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Domain')?></td>
      </tr>
 <?php
	if(!is_array($_REQUEST["percenthack"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["percenthack"] as $domain) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($domain)?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=percenthack"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ percenthack_print_form(void)
function percenthack_print_form() {
	global $error, $LANG;

	if(($_REQUEST["percentcount"] < count($_REQUEST["percenthack"]) - 1) and count($_REQUEST["percenthack"]) != 0)
	  $_REQUEST["percentcount"] = count($_REQUEST["percenthack"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">percenthack (<?=$LANG->_('Percent relaying domains', 4)?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Domains')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["percenthack_0"]); ?><input type="text" name="percenthack[0]" value="<?=$_REQUEST["percenthack"][0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["percentcount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["percenthack_$i"]); ?><input type="text" name="percenthack[<?=$i?>]" value="<?=$_REQUEST["percenthack"][$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&percentcount=<?=($_REQUEST["percentcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a domain, remove the value and save')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
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
	  case "modify";
	  case "add";
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
