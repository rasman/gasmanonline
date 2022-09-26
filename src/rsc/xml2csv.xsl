<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text"/>
	<xsl:template match="/">
		<xsl:text>Time, Name, Uptake, Delivered, CKT, ALV, VG, MUS, FAT, VEN</xsl:text>
		<xsl:for-each select="gasman/settings/results/result">
			<xsl:for-each select="agentresult">
				<xsl:text>
</xsl:text>
				<xsl:value-of select="concat(../@time,', ')"/>
				<xsl:value-of select="concat(@name,', ')"/>
				<xsl:value-of select="concat(@uptake,', ')"/>
				<xsl:value-of select="concat(@delivered,', ')"/>
				<xsl:value-of select="concat(compartments/compartment[@name='CKT']/@value,', ')"/>
				<xsl:value-of select="concat(compartments/compartment[@name='ALV']/@value,', ')"/>
				<xsl:value-of select="concat(compartments/compartment[@name='VRG']/@value,', ')"/>
				<xsl:value-of select="concat(compartments/compartment[@name='MUS']/@value,', ')"/>
				<xsl:value-of select="concat(compartments/compartment[@name='FAT']/@value,', ')"/>
				<xsl:value-of select="compartments/compartment[@name='VEN']/@value"/>
			</xsl:for-each>
		</xsl:for-each>
		<xsl:text>
</xsl:text>
	</xsl:template>
</xsl:stylesheet>
