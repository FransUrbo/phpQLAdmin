<?php
// control attribute plugin for
// percenthack (allowed domains for %-relaying)
//
// $Id: attrib.control.percenthack.inc,v 2.12 2003-11-20 08:01:36 turbo Exp $

function percenthack_check($type) {
	global $error, $percenthack, $view, $LANG;
	
	// check each field
	if(is_array($percenthack)) {
		foreach($percenthack as $key => $server) {
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

// init values
function percenthack_init($host) {
	global $_pql_control, $percenthack, $view;
	
	// fetch data from ldap server
	$percenthack = pql_control_get_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_GLOB_ATTR_PERCENTHACK"));
	
	if(is_null($percenthack))
	  return true;
}

function percenthack_print_view($host) {
	global $_pql_control, $percenthack, $view, $LANG;
	
	// init data
	percenthack_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">percenthack (<?=$LANG->_('Percent relaying domains', 4)?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Domain')?></td>
      </tr>
 <?php
	if(!is_array($percenthack)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($percenthack as $domain) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$domain?></td>
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

function percenthack_print_form() {
	global $attrib, $PHP_SELF, $error, $percenthack, $percentcount, $mxhost, $view, $LANG;

	if(($percentcount < count($percenthack) - 1) and count($percenthack) != 0)
	  $percentcount = count($percenthack) - 1;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">percenthack (<?=$LANG->_('Percent relaying domains', 4)?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Domains')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["percenthack_0"]); ?><input type="text" name="percenthack[0]" value="<?=$percenthack[0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $percentcount; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["percenthack_$i"]); ?><input type="text" name="percenthack[<?=$i?>]" value="<?=$percenthack[$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?$PHP_SELF?>?attrib=<?$attrib?>&percentcount=<?=($percentcount+1)?>&mxhost=<?=$mxhost?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional domain'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a domain, remove the value and save')?>.</td>
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

function percenthack_save($type, $host) {
	global $_pql_control, $attrib, $percenthack, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_PERCENTHACK"),
										 $percenthack)) 
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('percenthack options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('percenthack options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

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

function percenthack_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
