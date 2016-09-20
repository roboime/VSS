<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Controls" Type="Folder">
			<Item Name="SSL Detection Ball.ctl" Type="VI" URL="../SSL Detection Ball.ctl"/>
			<Item Name="SSL Detection Frame.ctl" Type="VI" URL="../SSL Detection Frame.ctl"/>
			<Item Name="SSL Detection Robot.ctl" Type="VI" URL="../SSL Detection Robot.ctl"/>
			<Item Name="SSL Geometry Camera Calibration.ctl" Type="VI" URL="../SSL Geometry Camera Calibration.ctl"/>
			<Item Name="SSL Geometry Field Size.ctl" Type="VI" URL="../SSL Geometry Field Size.ctl"/>
		</Item>
		<Item Name="GUI" Type="Folder">
			<Item Name="SSL Vision Client.vi" Type="VI" URL="../SSL Vision Client.vi"/>
			<Item Name="SSL Vision Log Player.vi" Type="VI" URL="../SSL Vision Log Player.vi"/>
		</Item>
		<Item Name="SubVI" Type="Folder">
			<Item Name="Decode Balls.vi" Type="VI" URL="../Decode Balls.vi"/>
			<Item Name="Decode Robots.vi" Type="VI" URL="../Decode Robots.vi"/>
			<Item Name="Decode SSL Wrapper Package.vi" Type="VI" URL="../Decode SSL Wrapper Package.vi"/>
			<Item Name="Double Decoder.vi" Type="VI" URL="../Double Decoder.vi"/>
			<Item Name="Field Draw Ball.vi" Type="VI" URL="../Field Draw Ball.vi"/>
			<Item Name="Field Draw Robot.vi" Type="VI" URL="../Field Draw Robot.vi"/>
			<Item Name="Field.vi" Type="VI" URL="../Field.vi"/>
			<Item Name="Float Decoder.vi" Type="VI" URL="../Float Decoder.vi"/>
			<Item Name="Length-delimited Decoder.vi" Type="VI" URL="../Length-delimited Decoder.vi"/>
			<Item Name="Proto Key And Type.vi" Type="VI" URL="../Proto Key And Type.vi"/>
			<Item Name="SSL Detection Ball.vi" Type="VI" URL="../SSL Detection Ball.vi"/>
			<Item Name="SSL Detection Frame.vi" Type="VI" URL="../SSL Detection Frame.vi"/>
			<Item Name="SSL Detection Robot.vi" Type="VI" URL="../SSL Detection Robot.vi"/>
			<Item Name="SSL Geometry Data.vi" Type="VI" URL="../SSL Geometry Data.vi"/>
			<Item Name="Varint Base128 Decoder.vi" Type="VI" URL="../Varint Base128 Decoder.vi"/>
			<Item Name="Varint Decoder uint32.vi" Type="VI" URL="../Varint Decoder uint32.vi"/>
			<Item Name="Varint Decoder.vi" Type="VI" URL="../Varint Decoder.vi"/>
		</Item>
		<Item Name="Varint Decoder uint64.vi" Type="VI" URL="../Varint Decoder uint64.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Draw Arc.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Arc.vi"/>
				<Item Name="Draw Circle by Radius.vi" Type="VI" URL="/&lt;vilib&gt;/picture/pictutil.llb/Draw Circle by Radius.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="LVPositionTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVPositionTypeDef.ctl"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Set Pen State.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Set Pen State.vi"/>
				<Item Name="UDP Multicast Open.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/UDP Multicast Open.vi"/>
				<Item Name="UDP Multicast Read-Only Open.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/UDP Multicast Read-Only Open.vi"/>
				<Item Name="UDP Multicast Read-Write Open.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/UDP Multicast Read-Write Open.vi"/>
				<Item Name="UDP Multicast Write-Only Open.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/UDP Multicast Write-Only Open.vi"/>
			</Item>
			<Item Name="_ChannelSupport.lvlib" Type="Library" URL="/&lt;resource&gt;/ChannelSupport/_ChannelSupport/_ChannelSupport.lvlib"/>
			<Item Name="ChannelProbePositionAndTitle.vi" Type="VI" URL="/&lt;resource&gt;/ChannelSupport/_ChannelSupport/ChannelProbePositionAndTitle.vi"/>
			<Item Name="ChannelProbeWindowStagger.vi" Type="VI" URL="/&lt;resource&gt;/ChannelSupport/_ChannelSupport/ChannelProbeWindowStagger.vi"/>
			<Item Name="PipeLogic.lvclass" Type="LVClass" URL="/&lt;resource&gt;/ChannelSupport/_ChannelSupport/PipeLogic/PipeLogic.lvclass"/>
			<Item Name="ProbeFormatting.vi" Type="VI" URL="/&lt;resource&gt;/ChannelSupport/_ChannelSupport/ProbeSupport/ProbeFormatting.vi"/>
			<Item Name="Stream-a[.](u8).lvlib" Type="Library" URL="/&lt;extravilib&gt;/ChannelInstances/Stream-a[.](u8).lvlib"/>
			<Item Name="Update Probe Details String.vi" Type="VI" URL="/&lt;resource&gt;/ChannelSupport/_ChannelSupport/ProbeSupport/Update Probe Details String.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
