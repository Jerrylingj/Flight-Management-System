const express = require('express');
const nodemailer = require('nodemailer');

const app = express();
const PORT = 3000;

// Middleware to parse JSON bodies
app.use(express.json());

// Configure the transporter for Nodemailer
const transporter = nodemailer.createTransport({
  host: 'smtp.qq.com', // QQ SMTP server
  port: 465,
  secure: true, // true for 465, false for other ports
  auth: {
    user: 'wateringtop@qq.com', // Your QQ email address
    pass: 'veezrihtkxmkbjae', // Your SMTP authorization code
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
    from: '"water" <wateringtop@qq.com>', // Sender address
    to: toEmail, // List of recipients
    subject: 'Your Verification Code', // Subject line
    text: `Your verification code is: ${code}`, // Plain text body
    // html: `<h1>Your verification code is: ${code}</h1>`, // HTML body (optional)
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
    return res.status(400).json({ message: 'Email and code are required.' });
  }

  try {
    await sendEmail(email, code);
    res.status(200).json({ message: 'Email sent successfully.' });
  } catch (error) {
    res.status(500).json({ message: 'Failed to send email.', error: error.toString() });
  }
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
