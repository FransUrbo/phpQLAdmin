<?php if($config["PQL_GLOB_CONTROL_USE"] && $defaultdomain) {
		// Link(s) for 'locals'
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "locals", $defaultdomain)){
			$locals = PQL_LANG_YES;
			if(!$config["PQL_GLOB_CONTROL_AUTOADDLOCALS"][$rootdn]) {
				$locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&rootdn=$rootdn&domain=$domain&type=del&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"remove $defaultdomain from locals\"></a>";
			} else {
				$locals_link = "&nbsp;";
			}
		} else {
			$locals = PQL_LANG_NO;
			if(!$config["PQL_GLOB_CONTROL_AUTOADDLOCALS"][$rootdn]) {
				$locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&rootdn=$rootdn&domain=$domain&type=add&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"add $defaultdomain to locals\"></a>";
			} else {
				$locals_link = "&nbsp;";
			}
		}
		
		// Link(s) for 'rcpthosts'
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "rcpthosts", $defaultdomain)){
			$rcpthosts = PQL_LANG_YES;
			if(!$config["PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS"][$rootdn])
			  $rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&rootdn=$rootdn&domain=$domain&type=del&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"remove $defaultdomain from rcpthosts\"></a>";
			else
			  $rcpthosts_link = "&nbsp;";
		} else {
			$rcpthosts = PQL_LANG_NO;
			if(!$config["PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS"][$rootdn])
			  $rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&rootdn=$rootdn&domain=$domain&type=add&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"add $defaultdomain to rcpthosts\"></a>";
			else
			  $rcpthosts_link = "&nbsp;";
		}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_LANG_OPTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Defined in locals</td>
        <td><?=$locals?></td>
        <td><?=$locals_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Defined in rcpthosts</td>
        <td><?=$rcpthosts?></td>
        <td><?=$rcpthosts_link?></td>
      </tr>
    </th>
  </table>

  <br><br>

<?php } // end if(PQL_GLOB_CONTROL_USE) ...
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
