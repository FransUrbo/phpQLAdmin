<?php if(pql_get_define("PQL_GLOB_CONTROL_USE") && $defaultdomain) {
		// Link(s) for 'locals'
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "locals", $defaultdomain)) {
			$locals = $LANG->_('Yes');
			if(!pql_get_define("PQL_GLOB_CONTROL_AUTOADDLOCALS", $rootdn)) {
				$alt = pql_complete_constant($LANG->_('Remove %domain% from %attribute%'), array('domain' => $defaultdomain, 'attribute' => 'locals'));
				$locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&rootdn=$rootdn&domain=$domain&type=del&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
			} else {
				$locals_link = "&nbsp;";
			}
		} else {
			$locals = $LANG->_('No');
			if(!pql_get_define("PQL_GLOB_CONTROL_AUTOADDLOCALS", $rootdn)) {
				$alt = pql_complete_constant($LANG->_('Add %what% to %attribute%'), array('what' => $defaultdomain, 'attribute' => 'locals'));
				$locals_link = "<a href=\"control_edit_attribute.php?attrib=locals&rootdn=$rootdn&domain=$domain&type=add&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
			} else {
				$locals_link = "&nbsp;";
			}
		}
		
		// Link(s) for 'rcpthosts'
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "rcpthosts", $defaultdomain)) {
			$rcpthosts = $LANG->_('Yes');
			if(!pql_get_define("PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS", $rootdn)) {
				$alt = pql_complete_constant($LANG->_('Remove %domain% from %attribute%'), array('domain' => $defaultdomain, 'attribute' => 'rcpthosts'));
				$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&rootdn=$rootdn&domain=$domain&type=del&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
			} else
			  $rcpthosts_link = "&nbsp;";
		} else {
			$rcpthosts = $LANG->_('No');
			if(!pql_get_define("PQL_GLOB_CONTROL_AUTOADDRCPTHOSTS", $rootdn)) {
				$alt = pql_complete_constant($LANG->_('Add %what% to %attribute%'), array('what' => $defaultdomain, 'attribute' => 'rcpthosts'));
				$rcpthosts_link = "<a href=\"control_edit_attribute.php?attrib=rcpthosts&rootdn=$rootdn&domain=$domain&type=add&set=$defaultdomain&submit=1&view=$view\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
			} else
			  $rcpthosts_link = "&nbsp;";
		}

		// Link(s) for 'smtroutes'
		if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, "smtproutes", $defaultdomain.'*')) {
			$smtproutes = $LANG->_('Yes');
			$alt = pql_complete_constant($LANG->_('Remove %domain% from %attribute%'), array('domain' => $defaultdomain, 'attribute' => 'smtproutes'));
			$smtproutes_link = "<a href=\"control_edit_attribute.php?rootdn=$rootdn&domain=$domain&attrib=smtproutes&type=del&set=$defaultdomain&view=$view\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
		} else {
			$smtproutes = $LANG->_('No');
			$alt = pql_complete_constant($LANG->_('Add %what% to %attribute%'), array('what' => $defaultdomain, 'attribute' => 'smtproutes'));
			$smtproutes_link = "<a href=\"control_edit_attribute.php?rootdn=$rootdn&domain=$domain&attrib=smtproutes&type=add&set=$defaultdomain&view=$view\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
		}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Options')?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Defined in %attribute%'), array('attribute' => 'locals')); ?></td>
        <td><?=$locals?></td>
        <td><?=$locals_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Defined in %attribute%'), array('attribute' => 'rcpthosts')); ?></td>
        <td><?=$rcpthosts?></td>
        <td><?=$rcpthosts_link?></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo pql_complete_constant($LANG->_('Defined in %attribute%'), array('attribute' => 'smtproutes')); ?></td>
        <td><?=$smtproutes?></td>
        <td><?=$smtproutes_link?></td>
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
