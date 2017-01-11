package com.globallogic.gl_smart.ui.view;

import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.List;


public class AppSpinnerAdapter<T> extends ArrayAdapter<T> {

    public interface AppSpinnerAdapterCallback<T> {
        String getText(T item);
        boolean isEnabled(T item);
    }

    public static class DefaultCallback<T> implements AppSpinnerAdapterCallback<T> {
        public String getText(T item) {
            return item.toString();
        }

        public boolean isEnabled(T item) {
            return true;
        }
    }

    private AppSpinnerAdapterCallback<T> mCallback;

    public AppSpinnerAdapter(Context context, List<T> data) {
        super(context, android.R.layout.simple_spinner_dropdown_item, data);

        mCallback = new DefaultCallback<>();
    }

    public AppSpinnerAdapter(Context context, List<T> data, AppSpinnerAdapterCallback<T> callback) {
        super(context, android.R.layout.simple_spinner_dropdown_item, data);

        mCallback = callback == null ? new DefaultCallback<T>() : callback;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View view = super.getView(position, convertView, parent);
        TextView textView = (TextView) view;
        if (mCallback != null) {
            textView.setText(mCallback.getText(getItem(position)));
        } else {
            textView.setText(getItem(position).toString());
        }
        return view;
    }

    @Override
    public View getDropDownView(int position, View convertView, ViewGroup parent) {
        View view = super.getDropDownView(position, convertView, parent);

        TextView textView = (TextView) view;

        if (mCallback != null) {
            textView.setText(mCallback.getText(getItem(position)));
        } else {
            textView.setText(getItem(position).toString());
        }

        if (isEnabled(position)) {
            textView.setTextColor(Color.BLACK);
        } else {
            textView.setTextColor(Color.GRAY);
        }
        return view;
    }

    @Override
    public boolean isEnabled(int position) {
        if (mCallback != null) {
            return mCallback.isEnabled(getItem(position));
        }

        return super.isEnabled(position);
    }
}
