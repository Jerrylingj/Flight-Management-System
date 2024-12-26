const express = require('express');
const nodemailer = require('nodemailer');

const app = express();
const PORT = 3000; // 运行的端口

app.use(express.json());

// Configure the transporter for Nodemailer
const transporter = nodemailer.createTransport({
	host: 'smtp.qq.com', // QQ SMTP server
	port: 465,
	secure: true,
	auth: {
		user: 'wateringtop@qq.com', // qq邮箱
		pass: 'veezrihtkxmkbjae', // 类似密码的东西
	},
});

// Verify the transporter configuration
transporter.verify((error, success) => {
	if (error) {
		console.error('Error configuring transporter:', error);
	} else {
		console.log('Server is ready to send emails');
	}
});

/**
 * Function to send an email with the provided code
 * @param {string} toEmail - Recipient's email address
 * @param {string} code - The code to send
 * @returns {Promise} - Resolves on success, rejects on error
 */
function sendEmail(toEmail, code) {
	const mailOptions = {
		from: '"altair" <wateringtop@qq.com>', // Sender address
		to: toEmail, // List of recipients
		subject: 'Your Verification Code', // Subject line
		html: `
		<p>Dear User,</p>
		<p>Thank you for registering with AltAir! We're excited to have you onboard.</p>
		<p>To complete your registration, please verify your email address by entering the code below:</p>
		<p><strong>Verification Code:</strong> ${code}</p>
		<p>Once you've entered this code, your account will be fully activated, and you can enjoy all the features our platform offers, including flight management, booking, and more!</p>
		<p>If you did not initiate this registration, please ignore this email. If you have any questions or need assistance, feel free to reach out to our support team by visiting our website at <a href="https://github.com/Terminal-Terrace">https://github.com/Terminal-Terrace</a>.</p>
		<p>About AltAir:</p>
		<p>At AltAir, we specialize in providing seamless and efficient flight management solutions for businesses and individuals alike. Our mission is to simplify travel booking and provide the best user experience possible. Learn more about our services at <a href="https://github.com/Jerrylingj/Flight-Management-System-Client">https://github.com/Jerrylingj/Flight-Management-System-Client</a>.</p>
		<p>Thank you for choosing AltAir!</p>
		<p>Best regards,<br/>The Terminal Terrace Team</p>
	  `,
	};

	return new Promise((resolve, reject) => {
		transporter.sendMail(mailOptions, (error, info) => {
			if (error) {
				console.error('Error sending email:', error);
				reject(error);
			} else {
				console.log('Email sent:', info.response);
				resolve(info);
			}
		});
	});
}

/**
 * POST /send-code
 * Body: { "email": "recipient@example.com", "code": "123456" }
 */
app.post('/api/send-code', async (req, res) => {
	const { email, code } = req.body;

	// Basic validation
	if (!email || !code) {
		return res
			.status(400)
			.json({ message: 'Email and code are required.' });
	}

	try {
		await sendEmail(email, code);
		res.status(200).json({ email, code });
	} catch (error) {
		res.status(500).json({
			message: 'Failed to send email.',
			error: error.toString(),
		});
	}
});

// Start the server
app.listen(PORT, () => {
	console.log(`Server is running on port ${PORT}`);
});
