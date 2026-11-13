package com.nokia.mj.impl.vmport;

import com.nokia.mj.impl.rt.DynamicProperty;
import com.nokia.mj.impl.rt.DynamicPropertyInfo;

import cc.nnproject.symbianext.PropertyExtensions;

import java.util.Hashtable;

public class SystemPropertiesExtensionHandler extends Hashtable {
	private static final String PROPERTY_HANDLER_CLASS_PREFIX = "com.nokia.mj.impl.rt.properties.";
	private Hashtable iOriginalProperties;

	public void storeOriginalProperties(Hashtable aOriginalProperties) {
		this.iOriginalProperties = aOriginalProperties;
	}

	public Object get(Object aName) {
		String propertyValue = null;
		String propertyName = (String) aName;
		try {
			if (propertyName != null && propertyName.startsWith("cc.nnproject.symbianext.")) {
				propertyValue = PropertyExtensions.getProperty(propertyName);
				if (propertyValue != null) {
					return propertyValue;
				}
			}
			
			propertyValue = (String) this.iOriginalProperties.get(propertyName);
			if ((propertyValue != null) && (propertyValue.length() > 0) && (propertyValue.charAt(0) == ':')) {
				DynamicPropertyInfo dynamicInfo = getDynamicProperty(propertyName, propertyValue);
				if (dynamicInfo != null) {
					String dynamicPropertyValue = dynamicInfo.getPropertyValue();
					if ((dynamicInfo.getIsPropertyValueStatic()) && (dynamicPropertyValue != null)) {
						this.iOriginalProperties.put(propertyName, dynamicPropertyValue);
					}
					return dynamicPropertyValue;
				}
				return null;
			}
			if (propertyValue != null) return propertyValue;
		} catch (Throwable t) {
			System.err.println("Error in system properties: Key=" + aName + ", Val ='" + propertyValue + "', Msg=" + t.getMessage());

			t.printStackTrace();
		}
		
		try {
			return com.symbian.j2me.System.getProperty(propertyName);
		} catch (Throwable ignored) {}
		
		return null;
	}

	private DynamicPropertyInfo getDynamicProperty(String aPropertyName, String aPropertyValue) throws Exception {
		String classNameEnd = aPropertyValue.substring(1);
		String className = PROPERTY_HANDLER_CLASS_PREFIX + classNameEnd;

		DynamicProperty dynamicProperty = null;

		dynamicProperty = (DynamicProperty) super.get(className);
		try {
			if (dynamicProperty == null) {
				Class clazz = Class.forName(className);
				dynamicProperty = (DynamicProperty) clazz.newInstance();
				super.put(className, dynamicProperty);
			}
			DynamicPropertyInfo dynamicInfo = new DynamicPropertyInfo(aPropertyName);

			dynamicProperty.getProperty(dynamicInfo);
			return dynamicInfo;
		} catch (ClassNotFoundException e) {
			System.err.println("Dynamic property handler " + className + " was not found for " + aPropertyName);
		}
		return null;
	}
}
